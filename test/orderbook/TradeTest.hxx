#pragma once

#include "../../src/orderbook/Types.hxx"

class TestTrade {
public:
  hermes::id_t makerId() { return 0; }
  hermes::id_t takerId() { return 0; }
  hermes::price_t price() { return 0; }
  hermes::quantity_t quantity() { return 0; }
};