#pragma once

#include "../../src/orderbook/Container.hxx"
#include "../../src/orderbook/Order.hxx"
#include "../../src/orderbook/Orderbook.hxx"

template <hermes::OrderLike Order, hermes::TradeLike Trade,
          hermes::ContainerLike<Order, Trade> Container,
          hermes::TradeListenerLike<Trade> TradeListener>
class TestOrderbook {
public:
  using bids_t = Container;
  using asks_t = Container;
  using order_id_t = typename Order::id_t;

  TestOrderbook() : bids_{}, asks_{} {};

  size_t size() { return 0; }
  bids_t getBids() { return bids_; }
  asks_t getAsks() { return asks_; }
  void insert(Order) {}
  void modify(Order) {}
  void remove(order_id_t) {}
  void trade(Order) {}

private:
  bids_t bids_;
  asks_t asks_;
};
