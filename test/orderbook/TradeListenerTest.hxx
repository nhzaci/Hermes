#pragma once

#include <vector>

#include "../../src/orderbook/Trade.hxx"

template <hermes::TradeLike Trade> class TestTradeListener {
public:
  void onTrade(Trade){};
  void onTrades(std::vector<Trade>){};
};