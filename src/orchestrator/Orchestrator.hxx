#pragma once

namespace hermes {

  template <typename DerivedOrchestrator, typename Gateway, typename OrderBook>
  class Orchestrator {
  public:
    /**
     * @brief Initializes configuration required to run the Orchestrator
     *
     */
    void init() {}
    void start() {}
    void stop() {}

  private:
    Orchestrator() {};
    friend DerivedOrchestrator;
  };

  // Examplar:
  //     A DerivedOrchestrator links all running services together into one 
  //     trading system where they communicate through a pub-sub pattern
  // class DerivedOrchestrator : public Orchestrator<DerivedOrchestrator> {
  // public:
  //   void start() {
  //     auto gateway{NasdaqGateway()};      // Market to send / receive feeds from
  //     auto orderbook{LimitOrderBook()};   // Simple LOB
  //     auto strategy{ArbitrageStrategy()}; // contains strategies to be executed (one or many)
  //     auto manager{OrderManager()};       // decides to send / no send
  //     auto risk{RiskStrategy()};          // some kind of risk strat
  //     auto router{LimitOrderRouter()};    // decides where to route for best returns
  //     
  //     // orderbook subscribes to one or more gateways to get its orders from
  //     orderbook.subscribe(gateway.subscribe());
  //     // orderbook publishes view to one or many strategies
  //     orderbook.publish(strategy);
  //     
  //     // strategy publishes to manager
  //     strategy.publish(manager);
  //     
  //     // manager gets some kind of risk strat
  //     manager.subscribe(risk);
  //     // manager publishes to router
  //     manager.publish(router);
  //     
  //     // router publishes to one or more gateway
  //     router.publish(gateway);
  //   }
  // };
};