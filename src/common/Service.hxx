#pragma once

#include "Pattern.hxx"

namespace hermes {

template <typename Derived> class Service : CRTP<Derived> {
public:
  void init();
  void start();
  void stop();
  void publish();
  void subscribe();
};

}; // namespace hermes