#include <gtest/gtest.h>

#include "ContainerTest.hxx"
#include "OrderTest.hxx"
#include "OrderbookTest.hxx"

#include "../../src/orderbook/Container.hxx"
#include "../../src/orderbook/Order.hxx"
#include "../../src/orderbook/Orderbook.hxx"

class OrderbookTestF : public ::testing::Test {};

TEST(OrderbookConcept, TestOrderbookWorks) {
  TestOrder order;
  TestOrderbook<TestOrder, TestContainer<TestOrder>> book;
  book.insert(order);
  book.modify(order);
  book.remove(order.id());
  book.trade(order);
}