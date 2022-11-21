#pragma once

#include "../common/Pattern.hxx"

#include <iostream>

namespace hermes {

/**
 * @brief A gateway provides a connection to an external network
 *
 * @tparam DerivedGateway Gateway with implemented subscription method
 */
template <typename DerivedGateway, typename Order, typename Stream>
class Gateway : CRTP<DerivedGateway> {
public:
  /**
   * @brief Subscribes to some gateway and returns an input stream of data which
   * can be read from later on
   *
   * @return Stream some data stream which can later be read
   */
  Stream subscribe() { return this->underlying().subscribe(); }

  /**
   * @brief Sends an order onto the exchange, Gateway would need to
   * encode the order according to the protocol the specific
   * gateway uses
   *
   * @param order Send some encoded order into a specific required format
   */
  void send(Order order) { return this->underlying().send(order); }

private:
  Gateway(){};
  friend DerivedGateway;
};

// TODO: Show Examplar of DerivedGateway
}; // namespace hermes