#pragma once

#include <concepts>
#include <iostream>

#include "Types.hxx"

namespace hermes {

template <typename TradeImpl>
concept TradeLike = requires(TradeImpl impl) {
  { impl.makerId() } -> std::convertible_to<id_t>;
  { impl.takerId() } -> std::convertible_to<id_t>;
  { impl.price() } -> std::convertible_to<price_t>;
  { impl.quantity() } -> std::convertible_to<quantity_t>;
};

class Trade {
public:
  Trade() : makerId_{}, takerId_{}, quantity_{}, price_{} {};
  Trade(id_t takerId, id_t makerId, quantity_t quantity, price_t price)
      : makerId_{makerId}, takerId_{takerId}, quantity_{quantity},
        price_{price} {};

  const price_t price() const { return price_; }
  const quantity_t quantity() const { return quantity_; }
  const id_t makerId() const { return makerId_; }
  const id_t takerId() const { return takerId_; }

  // TOOD: Add trade id into trade
  friend std::ostream &operator<<(std::ostream &os, const Trade &trade) {
    return os << "Trade(makerId=" << trade.makerId()
              << ",takerId=" << trade.takerId() << ",price=" << trade.price()
              << ",quantity=" << trade.quantity() << ")";
  }

private:
  // id_t tradeId_; // TODO: implement trade id, using a factory probably
  id_t makerId_;
  id_t takerId_;
  quantity_t quantity_;
  price_t price_;
};

}; // namespace hermes