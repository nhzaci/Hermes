#pragma once

#include <concepts>
#include <cstdint>
#include <iostream>

#include "OrderType.hxx"

namespace hermes {

template <typename OrderImpl>
concept OrderLike = requires(OrderImpl impl) {
  typename OrderImpl::price_t;
  typename OrderImpl::quantity_t;
  typename OrderImpl::id_t;
  typename OrderImpl::exch_id_t;
  typename OrderImpl::type_t;

  { impl.isBuy() } -> std::convertible_to<bool>;
  { impl.price() } -> std::convertible_to<typename OrderImpl::price_t>;
  { impl.exchangeId() } -> std::convertible_to<typename OrderImpl::exch_id_t>;
  { impl.id() } -> std::convertible_to<typename OrderImpl::id_t>;
  { impl.quantity() } -> std::convertible_to<typename OrderImpl::quantity_t>;
  {
    impl.setQuantity(typename OrderImpl::quantity_t{})
    } -> std::convertible_to<void>;
  { impl.orderType() } -> std::convertible_to<typename OrderImpl::type_t>;
};

class Order {
public:
  using price_t = uint64_t;
  using quantity_t = uint64_t;
  using id_t = uint64_t;
  using exch_id_t = uint64_t;
  using type_t = OrderType;

  Order()
      : id_{}, price_{}, quantity_{}, isBuy_{false}, exchangeId_{},
        orderType_{} {};

  Order(id_t id, price_t price, quantity_t quantity, bool isBuy,
        exch_id_t exchangeId)
      : id_{id}, price_{price}, quantity_{quantity}, isBuy_{isBuy},
        exchangeId_{exchangeId}, orderType_{OrderType::Limit} {};

  Order(id_t id, price_t price, quantity_t quantity, bool isBuy,
        exch_id_t exchangeId, OrderType type)
      : id_{id}, price_{price}, quantity_{quantity}, isBuy_{isBuy},
        exchangeId_{exchangeId}, orderType_{type} {};

  bool isBuy() const { return isBuy_; };
  price_t price() const { return price_; };
  quantity_t quantity() const { return quantity_; };
  id_t id() const { return id_; };
  exch_id_t exchangeId() const { return exchangeId_; };
  type_t orderType() const { return orderType_; }

  void setQuantity(quantity_t newQuantity) { quantity_ = newQuantity; };

  friend std::ostream &operator<<(std::ostream &os, const Order &order) {
    return os << "Order(id=" << order.id_ << ";price=" << order.price_
              << ";exchId=" << order.exchangeId_
              << ";quantity=" << order.quantity_
              << ";isBuy=" << (order.isBuy_ ? "true" : "false")
              << ";orderType=" << order.orderType_ << ")";
  }

private:
  id_t id_;
  price_t price_;
  quantity_t quantity_;
  bool isBuy_;
  exch_id_t exchangeId_;
  type_t orderType_;
};

}; // namespace hermes