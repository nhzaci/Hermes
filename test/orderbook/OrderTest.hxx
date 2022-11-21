#pragma once

#include "../../src/orderbook/Order.hxx"

class TestOrder {
public:
  using price_t = uint64_t;
  using quantity_t = uint64_t;
  using id_t = uint64_t;
  using exch_id_t = uint64_t;
  using type_t = hermes::OrderType;

  bool isBuy() { return true; }
  price_t price() { return 1; }
  quantity_t quantity() { return 1; }
  void setQuantity(quantity_t) {}
  exch_id_t exchangeId() { return 1; }
  id_t id() { return 1; }
  type_t orderType() { return hermes::OrderType::Limit; }
};