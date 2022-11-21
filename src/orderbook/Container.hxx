#pragma once

#include "Order.hxx"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hermes {

template <typename ContainerImpl, typename Order,
          typename Compare = std::less<typename Order::price_t>>
concept ContainerLike = OrderLike<Order> && requires(ContainerImpl impl) {
  typename ContainerImpl::itr_t;
  typename ContainerImpl::cmp_t;

  { impl.size() } -> std::convertible_to<size_t>;
  { impl.bestPrice() } -> std::convertible_to<typename Order::price_t>;
  { impl.contains(typename Order::id_t{}) } -> std::convertible_to<bool>;
  { impl.insert(Order{}) } -> std::convertible_to<void>;
  { impl.modify(Order{}) } -> std::convertible_to<void>;
  {
    impl.find(typename Order::id_t{})
    } -> std::convertible_to<typename ContainerImpl::itr_t>;
  { impl.remove(typename Order::id_t{}) } -> std::convertible_to<void>;
  // TODO: Figure out how to pass a lvalue in a concept function
  // { impl.cross(std::declval<Order>()) } -> std::convertible_to<void>;
} &&(std::same_as<Compare, std::less<typename Order::price_t>> ||
     std::same_as<Compare, std::greater<typename Order::price_t>>);

/**
 * @brief RBTreeContainer is a data structure that contains two data structures.
 * 1. Red Black Tree. The key of each node is the price level, the value of
 * each node is a doubly-linked list
 * 2. Hash Map. Key is the id of an order and the value is the iterator of the
 * node inside its doubly-linked list.
 *
 * @tparam Order    Order must adhere to OrderLike concept
 * @tparam Compare  Contains algorithm for sorting of orders within container
 */
template <OrderLike Order,
          typename Compare = std::less<typename Order::price_t>>
class RBTreeContainer {
public:
  using order_id_t = typename Order::id_t;
  using order_price_t = typename Order::price_t;
  using order_quantity_t = typename Order::quantity_t;
  using order_exch_id_t = typename Order::exch_id_t;
  using order_type_t = typename Order::type_t;

  using price_t = typename Order::price_t;
  using list_itr_t = typename std::list<Order>::iterator;
  using itr_t = list_itr_t;
  using cmp_t = Compare;
  using container_t = std::map<price_t, std::list<Order>, cmp_t>;
  using id_map_t = std::unordered_map<order_id_t, list_itr_t>;

  RBTreeContainer() : priceLevels_{}, orderMap_{}, size_{}, cmp_fn_{} {}

  /**
   * @brief Returns current size
   */
  size_t size() const { return size_; }

  /**
   * @brief Checks if id exists in hash table
   */
  bool contains(order_id_t id) { return orderMap_.contains(id); }

  /**
   * @brief Finds a node, deferencing might lead to a
   * segmentation fault if the id does not exist since it would be dereferencing
   * the end of a std::list itr
   *
   * @return Returns an iterator to the order within its linked list
   */
  itr_t find(order_id_t id) const { return orderMap_.find(id)->second; }

  /**
   * @brief Inserts an order into the tree and updates hash table
   */
  void insert(const Order &order) {
    // intentional get or create
    auto &level = priceLevels_[order.price()];
    level.push_back(order);

    // intentional get or create
    orderMap_[order.id()] = --level.end();

    // incr size
    size_++;
  }

  /**
   * @brief Inserts an order into the tree and updates hash table
   */
  void insert(Order &&order) {
    auto id = order.id();

    // intentional get or create
    auto &level = priceLevels_[order.price()];
    level.push_back(std::move(order));

    // intentional get or create
    orderMap_[id] = --level.end();

    // incr size
    size_++;
  }

  void modify(const Order &newOrder) {
    if (newOrder.quantity() == 0) {
      remove(newOrder.id());
      return;
    }

    auto mapItr = orderMap_.find(newOrder.id());
    auto &currOrder = *(mapItr->second);

    // if it's just an update in quantity, no need to do much
    if (currOrder.price() == newOrder.price()) {
      currOrder.setQuantity(newOrder.quantity());
      return;
    }

    // otherwise, price has changed, move it into a new place
    remove(newOrder.id());
    insert(newOrder);
  }

  void modify(Order &&newOrder) {
    if (newOrder.quantity() == 0) {
      remove(newOrder.id());
      return;
    }

    auto mapItr = orderMap_.find(newOrder.id());
    auto &currOrder = *(mapItr->second);

    if (currOrder.price() == newOrder.price()) {
      currOrder.setQuantity(newOrder.quantity());
      return;
    }

    remove(newOrder.id());
    insert(std::move(newOrder));
  }

  /**
   * @brief Removes order and removes price level if level is empty
   */
  void remove(order_id_t id) {
    auto mapItr = orderMap_.find(id);
    auto orderItr = mapItr->second;

    auto levelItr = priceLevels_.find(orderItr->price());
    auto level = levelItr->second;

    // erase item from level and order map
    level.erase(orderItr);
    orderMap_.erase(id);

    // if level is empty, remove level
    if (level.size() == 0) {
      priceLevels_.erase(levelItr);
    }

    // decrement size
    size_--;
  }

  // TODO: Abstract crossing out to a strategy, there are many strategies
  // for crossing an order. Currently it's FIFO, try passing a strategy
  // where I'm able to cross with multiple different possibilities
  void cross(Order &oppSideOrder) {
    // while opp of comparator or equal, pop orders from list,
    // advancing price level itr while necessary
    auto priceLevelItr = priceLevels_.begin();
    while (priceLevelItr != priceLevels_.end() and
           (priceLevelItr->first == oppSideOrder.price() or
            cmp_fn_(priceLevelItr->first, oppSideOrder.price())) and
           oppSideOrder.quantity() > 0) {
      // iterate through price level list
      auto levelList = priceLevelItr->second;
      auto levelListItr = levelList.begin();

      // iterate through levels until we reach 0 quantity left
      while (levelListItr != levelList.end() and oppSideOrder.quantity() > 0) {
        // get curr order
        const auto &currOrder = *levelListItr;
        // check quantity fulfilled
        auto quantityFulfilled =
            std::min(oppSideOrder.quantity(), currOrder.quantity());

        // set quantity of the order
        oppSideOrder.setQuantity(oppSideOrder.quantity() - quantityFulfilled);

        levelListItr++;

        // modify current pointed to order
        if (currOrder.quantity() <= quantityFulfilled) {
          remove(currOrder.id());
        } else {
          orderMap_[currOrder.id()]->setQuantity(currOrder.quantity() -
                                                 quantityFulfilled);
        }
      }

      // get next best price
      priceLevelItr = priceLevels_.begin();
    }
  }

  price_t bestPrice() const { return priceLevels_.begin()->first; }

  friend std::ostream &operator<<(std::ostream &os,
                                  const RBTreeContainer &container) {
    for (auto [key, list] : container.priceLevels_) {
      os << "\n===\nprice level: " << key << "\n";

      for (auto item : list) {
        os << item << "\n";
      }

      os << "===\n";
    }

    return os;
  }

private:
  container_t priceLevels_;
  id_map_t orderMap_;
  size_t size_;
  cmp_t cmp_fn_;
};

}; // namespace hermes
