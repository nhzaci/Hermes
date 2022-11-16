#pragma once

#include <iostream>

namespace hermes {

// TODO: Add more order types
enum class OrderType : uint8_t { Limit, FillOrKill, Market };

std::ostream &operator<<(std::ostream &os, const OrderType &type) {
  switch (type) {
  case OrderType::Limit:
    return os << "Limit";
  case OrderType::FillOrKill:
    return os << "FillOrKill";
  case OrderType::Market:
    return os << "Market";
  }
  return os;
}

};