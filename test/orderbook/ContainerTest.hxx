#pragma once

#include "OrderTest.hxx"

#include "../../src/orderbook/Container.hxx"
#include "../../src/orderbook/Order.hxx"

template <hermes::OrderLike Order> class TestContainer {
public:
  using order_id_t = typename Order::id_t;
  using price_t = typename Order::price_t;
  using cmp_t = typename std::less<price_t>;
  using itr_t = void;

  itr_t find(order_id_t id) {}
  size_t size() { return 0; }
  bool contains(order_id_t) { return false; }
  void insert(Order) {}
  void modify(Order) {}
  void remove(order_id_t) {}
  void cross(Order) {}
  price_t bestPrice() { return 0; }
};
