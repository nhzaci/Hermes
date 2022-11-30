#include <gtest/gtest.h>

#include <iostream>

#include "ContainerTest.hxx"
#include "OrderTest.hxx"
#include "OrderbookTest.hxx"
#include "TradeListenerTest.hxx"
#include "TradeTest.hxx"

#include "../../src/orderbook/Container.hxx"
#include "../../src/orderbook/Order.hxx"
#include "../../src/orderbook/Orderbook.hxx"
#include "../../src/orderbook/Types.hxx"

class OrderbookTestF : public ::testing::Test {
public:
  OrderbookTestF() : book{} {};

  hermes::Orderbook<hermes::Order, hermes::Trade,
                    hermes::RBTreeContainer<hermes::Order, hermes::Trade>,
                    hermes::RBTreeContainer<hermes::Order, hermes::Trade,
                                            std::greater<hermes::price_t>>,
                    hermes::TradeStore<hermes::Trade>>
      book;
  hermes::exch_id_t exchangeId = 10;

  /**
   * Book display:
   *      1011, id:2 q:3, id: 3, q:10
   * ask  1010, id:1 q:4
   * ---------
   * spread: 2
   * ---------
   * bid  1008 id:4 q:10
   *      1007 id:5 q:2
   */
  void SetUp() override {
    using namespace hermes;

    // asks
    book.insert(Order(1, 1010, 4, false, exchangeId));
    book.insert(Order(2, 1011, 3, false, exchangeId));
    book.insert(Order(3, 1011, 10, false, exchangeId));

    // bids
    book.insert(Order(4, 1008, 10, true, exchangeId));
    book.insert(Order(5, 1007, 2, true, exchangeId));
  }

  void TearDown() override {}
};

TEST(OrderbookConcept, TestOrderbookWorks) {
  TestOrder order;
  TestOrderbook<TestOrder, TestTrade, TestContainer<TestOrder, TestTrade>,
                TestTradeListener<TestTrade>>
      book;
  book.insert(order);
  book.modify(order);
  book.remove(order.id());
  book.trade(order);
}

TEST_F(OrderbookTestF, CorrectSpread) { EXPECT_EQ(book.bidAskSpread(), 2); }

TEST_F(OrderbookTestF, CanInsert) {
  book.insert(hermes::Order(6, 1200, 10, false, exchangeId));
  EXPECT_TRUE(book.asks().contains(6));
}

TEST_F(OrderbookTestF, CrossesOrder) {
  book.insert(hermes::Order(6, 1006, 14, false, exchangeId));
  auto listener = book.trade_listener();
  auto trades = listener.trades();

  bool containsMaker4 = false;
  bool containsMaker5 = false;
  bool containsTaker6 = false;

  for (auto t : trades) {
    if (t.makerId() == 4) {
      EXPECT_EQ(t.price(), 1008);
      EXPECT_EQ(t.quantity(), 10);
      containsMaker4 = true;
    }

    if (t.makerId() == 5) {
      EXPECT_EQ(t.price(), 1007);
      EXPECT_EQ(t.quantity(), 2);
      containsMaker5 = true;
    }

    if (t.takerId() == 6) {
      containsTaker6 = true;
    }
  }

  EXPECT_TRUE(containsMaker4);
  EXPECT_TRUE(containsMaker5);
  EXPECT_TRUE(containsTaker6);
  EXPECT_TRUE(book.asks().contains(6));
  EXPECT_FALSE(book.bids().contains(4));

  EXPECT_EQ(book.asks().find(6)->quantity(), 2);
}