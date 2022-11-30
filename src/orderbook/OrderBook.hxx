#pragma once

#include "Container.hxx"
#include "Order.hxx"
#include "Trade.hxx"
#include "TradeListener.hxx"
#include "Types.hxx"

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace hermes {

template <typename OrderbookImpl, typename BidsContainer,
          typename AsksContainer, typename Order, typename TradeListener,
          typename Trade>
concept OrderbookLike = OrderLike<Order> &&
    ContainerLike<BidsContainer, Order, Trade> &&
    ContainerLike<AsksContainer, Order, Trade> && TradeLike<Trade> &&
    TradeListenerLike<TradeListener, Trade> && requires(OrderbookImpl impl) {
  typename OrderbookImpl::bids_t;
  typename OrderbookImpl::asks_t;

  { impl.symbol() } -> std::convertible_to<std::string>;
  { impl.size() } -> std::convertible_to<size_t>;
  { impl.trade_listener() } -> std::convertible_to<TradeListener>;
  { impl.getBids() } -> std::convertible_to<typename OrderbookImpl::bids_t>;
  { impl.getAsks() } -> std::convertible_to<typename OrderbookImpl::asks_t>;
  { impl.insert(Order{}) } -> std::convertible_to<void>;
  { impl.modify(Order{}) } -> std::convertible_to<void>;
  { impl.remove(Order::id_t) } -> std::convertible_to<void>;
  { impl.trade(Order{}) } -> std::convertible_to<void>;
};

template <OrderLike Order, TradeLike Trade,
          ContainerLike<Order, Trade> AsksContainer,
          ContainerLike<Order, Trade, std::greater<price_t>> BidsContainer,
          TradeListenerLike<Trade> TradeListener>
class Orderbook {
public:
  using asks_t = AsksContainer;
  using bids_t = BidsContainer;
  using symbol_t = std::string;
  using trade_listener_t = TradeListener;

  Orderbook() : bids_{}, asks_{}, trade_listener_{}, symbol_{"Undefined"} {};
  Orderbook(std::string symbol)
      : bids_{}, asks_{}, trade_listener_{}, symbol_{symbol} {};

  /**
   * @brief Templated function for crossing buy / sell order
   */
  template <bool isBuy> std::vector<Trade> cross(Order &);

  template <> std::vector<Trade> cross<true>(Order &buyOrder) {
    auto bestSellPrice = asks_.bestPrice();
    if (buyOrder.price() < bestSellPrice)
      return {};
    return asks_.cross(buyOrder);
  }

  template <> std::vector<Trade> cross<false>(Order &sellOrder) {
    auto bestBuyPrice = bids_.bestPrice();
    if (bestBuyPrice < sellOrder.price())
      return {};
    return bids_.cross(sellOrder);
  }

  /**
   * @brief Insert order into bid or ask container, crosses order and reports
   * trades to trade listener and inserts if available
   *  TODO: Optimize, branching before inserting doesn't look the most elegant
   */
  void insert(Order &&order) {
    if (order.isBuy()) {
      trade_listener_.onTrades(cross<true>(order));
      if (order.quantity() > 0)
        bids_.insert(std::move(order));
    } else {
      trade_listener_.onTrades(cross<false>(order));
      if (order.quantity() > 0)
        asks_.insert(std::move(order));
    }
  }

  /**
   * @brief Insert order into bid or ask container, crosses order and reports
   * trades to trade listener and inserts if available
   *  TODO: Optimize, branching before inserting doesn't look the most elegant
   */
  void insert(Order &order) {
    if (order.isBuy()) {
      trade_listener_.onTrades(cross<true>(order));
      if (order.quantity() > 0)
        bids_.insert(order);
    } else {
      trade_listener_.onTrades(cross<false>(order));
      if (order.quantity() > 0)
        asks_.insert(order);
    }
  }

  /**
   * @brief Insert order into bid or ask container
   */
  void modify(Order &&order) {
    getBook(order.isBuy()).modify(std::move(order));
  }

  /**
   * @brief Remove order from bid or ask container
   */
  void remove(id_t id, bool isBuyOrder) {
    if (isBuyOrder)
      bids_.remove(id);
    else
      asks_.remove(id);
  }

  /**
   * @brief Bid ask spread is the price discrepancy between bid and ask
   */
  price_t bidAskSpread() { return asks_.bestPrice() - bids_.bestPrice(); }

  const bids_t &bids() const { return bids_; }
  const asks_t &asks() const { return asks_; }
  const std::string &symbol() const { return symbol_; }
  const size_t size() const { return bids_.size() + asks_.size(); }
  const trade_listener_t &trade_listener() const { return trade_listener_; }

private:
  bids_t bids_;
  asks_t asks_;
  trade_listener_t trade_listener_;
  std::string symbol_;
};

}; // namespace hermes
