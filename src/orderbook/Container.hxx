#pragma once

#include "../common/Pattern.hxx"
#include "Order.hxx"

#include <cstdint>
#include <functional>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

namespace hermes {

template <typename ContainerImpl, typename Order,
          typename Compare = std::less<typename Order::price_t>>
concept ContainerLike = OrderLike<Order> && requires(ContainerImpl impl) {
  { impl.size() } -> std::convertible_to<size_t>;
  { impl.bestPrice() } -> std::convertible_to<typename Order::price_t>;
  { impl.contains(typename Order::id_t{}) } -> std::convertible_to<bool>;
  { impl.insert(Order{}) } -> std::convertible_to<void>;
  { impl.modify(Order{}) } -> std::convertible_to<void>;
  { impl.find(typename Order::id_t{}) } -> std::convertible_to<Order>;
  { impl.remove(typename Order::id_t{}) } -> std::convertible_to<void>;
  { impl.trade(Order{}) } -> std::convertible_to<void>;
} &&(std::same_as<Compare, std::less<typename Order::price_t>> ||
     std::same_as<Compare, std::greater<typename Order::price_t>>);

template <OrderLike Order,
          typename Compare = std::less<typename Order::price_t>>
class RBTreeContainer {
public:
  using price_t = typename Order::price_t;
  using list_itr_t = typename std::list<Order>::iterator;
  using order_id_t = typename Order::id_t;
  using cmp_t = Compare;
  using container_t = std::map<price_t, std::list<Order>, cmp_t>;
  using id_map_t = std::unordered_map<order_id_t, list_itr_t>;

  RBTreeContainer() : priceLevels_{}, orderMap_{}, size_{} {}

  size_t size() const { return size_; }

  bool contains(order_id_t id) { return orderMap_.contains(id); }

  const Order &find(order_id_t id) const {
    return *(orderMap_.find(id)->second);
  }

  // TODO: Duplicate checking
  void insert(const Order &order) {
    // intentional get or create
    auto &level = priceLevels_[order.price()];
    level.push_back(order);

    // intentional get or create
    orderMap_[order.id()] = --level.end();

    // incr size
    size_++;
  }

  // TODO: Duplicate checking
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
    remove(newOrder.id());
    insert(newOrder);
  }

  void modify(Order &&newOrder) {
    remove(newOrder.id());
    insert(std::move(newOrder));
  }

  // TODO: Error checking
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

  void trade(Order &&order) {}

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
};

}; // namespace hermes
