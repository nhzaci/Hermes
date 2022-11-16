#pragma once

#include "../common/Pattern.hxx"
#include "Container.hxx"
#include "Order.hxx"

#include <functional>
#include <iostream>
#include <tuple>
#include <vector>

namespace hermes {

template <typename OrderbookImpl, typename BidsContainer,
          typename AsksContainer, typename Order>
concept OrderbookLike = OrderLike<Order> &&
    ContainerLike<BidsContainer, Order> &&
    ContainerLike<AsksContainer, Order> && requires(OrderbookImpl impl) {
  typename OrderbookImpl::bids_t;
  typename OrderbookImpl::asks_t;

  { impl.size() } -> std::convertible_to<size_t>;
  { impl.getBids() } -> std::convertible_to<typename OrderbookImpl::bids_t>;
  { impl.getAsks() } -> std::convertible_to<typename OrderbookImpl::asks_t>;
  { impl.insert(Order{}) } -> std::convertible_to<void>;
  { impl.modify(Order{}) } -> std::convertible_to<void>;
  { impl.remove(Order::id_t) } -> std::convertible_to<void>;
  { impl.trade(Order{}) } -> std::convertible_to<void>;
};

template <
    OrderLike Order, ContainerLike<Order> AsksContainer,
    ContainerLike<Order, std::greater<typename Order::price_t>> BidsContainer>
class Orderbook {
public:
  using asks_t = AsksContainer;
  using bids_t = BidsContainer;
  using order_id_t = typename Order::id_t;

  Orderbook() : bids_{}, asks_{} {};

  size_t size() { return size_; }

  void insert(Order &&order) {}
  void insert(const Order &order) {}

  void modify(Order &&order) {}

  void remove(order_id_t id) {}

  void trade(const Order &order) {}
  void trade(Order &&order) {}

private:
  bids_t bids_;
  asks_t asks_;
  size_t size_;

  /**
   * @brief Bid ask spread crossing
   */
  void cross() {}
};

}; // namespace hermes
