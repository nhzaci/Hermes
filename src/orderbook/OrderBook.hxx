#pragma once

#include "Container.hxx"
#include "Order.hxx"

#include <functional>
#include <iostream>
#include <memory>
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
  using order_price_t = typename Order::price_t;
  using order_quantity_t = typename Order::quantity_t;
  using order_exch_id_t = typename Order::exch_id_t;
  using order_type_t = typename Order::type_t;

  Orderbook() : bids_{}, asks_{} {};

  size_t size() { return bids_.size() + asks_.size(); }

  void insert(Order &&order) {
    if (order.isBuy())
      bids_.insert(std::move(order));
    else
      asks_.insert(std::move(order));
  }

  void insert(const Order &order) {
    if (order.isBuy())
      bids_.insert(order);
    else
      asks_.insert(order);
  }

  void modify(Order &&order) {
    if (order.isBuy())
      bids_.modify(std::move(order));
    else
      asks_.modify(std::move(order));
  }

  void remove(order_id_t id, bool isBuyOrder) {
    if (isBuyOrder)
      bids_.remove(id);
    else
      asks_.remove(id);
  }

  template <bool isBuy> void cross(Order &);

  template <> void cross<true>(Order &buyOrder) {
    auto bestSellPrice = asks_.bestPrice();
    if (buyOrder.price() < bestSellPrice)
      return;
    asks_.cross(buyOrder);
  }

  template <> void cross<false>(Order &sellOrder) {
    auto bestBuyPrice = bids_.bestPrice();
    if (bestBuyPrice < sellOrder.price())
      return;
    bids_.cross(sellOrder);
  }

private:
  bids_t bids_;
  asks_t asks_;
};

}; // namespace hermes
