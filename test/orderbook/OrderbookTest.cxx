#include <gtest/gtest.h>

#include "ContainerTest.hxx"
#include "OrderTest.hxx"
#include "OrderbookTest.hxx"

TEST(OrderbookConcept, TestOrderbookWorks) {
  TestOrder order;
  TestOrderbook<TestOrder, TestContainer<TestOrder>> book;
  book.insert(order);
  book.modify(order);
  book.remove(order.id());
  book.trade(order);
}