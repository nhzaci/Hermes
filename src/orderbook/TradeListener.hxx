#pragma once

#include <concepts>
#include <iterator>
#include <memory>
#include <vector>

#include "Trade.hxx"

namespace hermes {

template <typename TradeListenerImpl, typename Trade>
concept TradeListenerLike = TradeLike<Trade> &&
    requires(TradeListenerImpl impl) {
  { impl.onTrade(Trade{}) } -> std::convertible_to<void>;
  { impl.onTrades(std::vector<Trade>{}) } -> std::convertible_to<void>;
};

template <TradeLike Trade> class TradeStore {
public:
  void onTrade(Trade &&newTrade) { trades_.push_back(std::move(newTrade)); }

  void onTrade(const Trade &newTrade) { trades_.push_back(newTrade); }

  void onTrades(const std::vector<Trade> &newTrades) {
    trades_.insert(trades_.end(), newTrades.begin(), newTrades.end());
  }

  void onTrades(std::vector<Trade> &&newTrades) {
    trades_.insert(trades_.end(), std::make_move_iterator(newTrades.begin()),
                   std::make_move_iterator(newTrades.end()));
  }

  const std::vector<Trade> &trades() const { return trades_; }

private:
  std::vector<Trade> trades_;
};

}; // namespace hermes