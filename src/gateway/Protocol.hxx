#pragma once

namespace hermes {

  /**
   * @brief A protocol defines some way of allowing a Gateway to
   * perform some function. There could be multiple different protocols
   * that a single Gateway uses in order to perform its functions
   *
   * @tparam Input Data from which
   * @tparam Input Some class to encode into
   */
  template <typename DerivedProtocol, typename Input, typename Output>
  class Protocol {
    /**
     * @brief Serializes some input type (from trading system) into required
     * output (to gateway) type according to implementation of protocol
     */
    Output serialize(Input input) {
      DerivedProtocol& derived = static_cast<DerivedProtocol&>(*this);
      return derived.encode(input);
    }

    /**
     * @brief Deserialize some output type (from the gateway) into required
     * input type (to trading system) according to implementation of protocol
     */
    Input deserialize(Output output) {
      DerivedProtocol& derived = static_cast<DerivedProtocol&>(*this);
      return derived.decode(output);
    }

  private:
    Protocol() {};
    friend DerivedProtocol;
  };

  // TODO: Write an examplar of DerivedProtocol

};