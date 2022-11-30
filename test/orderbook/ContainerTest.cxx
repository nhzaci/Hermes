#include <gtest/gtest.h>

#include <memory>

#include "ContainerTest.hxx"
#include "OrderTest.hxx"
#include "TradeTest.hxx"

#include "../../src/orderbook/Container.hxx"
#include "../../src/orderbook/Order.hxx"
#include "../../src/orderbook/Orderbook.hxx"

class RBTreeContainerTestF : public ::testing::Test {
public:
  RBTreeContainerTestF() : container_{} {}

  hermes::RBTreeContainer<hermes::Order, hermes::Trade> container_;

  void SetUp() override {
    using namespace hermes;
    container_.insert(Order(1, 2, 3, false, 4));
    container_.insert(Order(2, 3, 4, false, 4));
    container_.insert(Order(3, 4, 5, false, 4));
    container_.insert(Order(4, 5, 6, false, 4));
  }

  void TearDown() override {}
};

TEST(ContainerConcept, TestContainerWorks) {
  using namespace hermes;
  TestOrder order;
  TestContainer<TestOrder, TestTrade> orderContainer;
  orderContainer.insert(order);
  orderContainer.modify(order);
  orderContainer.cross(order);
  orderContainer.remove(2);
}

TEST_F(RBTreeContainerTestF, InitialSize) { ASSERT_EQ(container_.size(), 4); }

TEST_F(RBTreeContainerTestF, ContainsOrder) {
  ASSERT_TRUE(container_.contains(2));
  ASSERT_FALSE(container_.contains(5));
}

TEST_F(RBTreeContainerTestF, RemoveOrderAndCheckSizeAndContains) {
  container_.remove(2);
  ASSERT_EQ(container_.size(), 3);
  ASSERT_FALSE(container_.contains(2));
}

TEST_F(RBTreeContainerTestF, InsertAndRemoveOrder) {
  container_.insert(hermes::Order(10, 20, 30, true, 4));
  ASSERT_EQ(container_.size(), 5);

  container_.remove(10);
  ASSERT_EQ(container_.size(), 4);
}

TEST_F(RBTreeContainerTestF, FindOrder) {
  auto order = container_.find(2);
  ASSERT_EQ(order->id(), 2);
}

TEST_F(RBTreeContainerTestF, ModifyOrder) {
  auto id = 2;
  auto exchId = 4;
  auto newPrice = 100000;
  auto newQuantity = 20014;
  auto newOrder = hermes::Order(id, newPrice, newQuantity, true, exchId);

  container_.modify(std::move(newOrder));

  auto findRes = container_.find(id);
  ASSERT_EQ(findRes->id(), id);
  ASSERT_EQ(findRes->price(), newPrice);
  ASSERT_EQ(findRes->quantity(), newQuantity);
  ASSERT_EQ(findRes->exchangeId(), exchId);
}

TEST_F(RBTreeContainerTestF, BestPrice) {
  ASSERT_EQ(container_.bestPrice(), 2);
}

TEST_F(RBTreeContainerTestF, CrossFullyFilledOrder) {
  auto buyOrder = hermes::Order(10, 2, 3, true, 4);

  container_.cross(buyOrder);
  EXPECT_EQ(buyOrder.quantity(), 0);

  auto containsOrder1 = container_.contains(1);
  EXPECT_FALSE(containsOrder1);
}

TEST_F(RBTreeContainerTestF, CrossPartiallyFilledOrder) {
  auto buyOrder = hermes::Order(10, 2, 2, true, 4);

  container_.cross(buyOrder);
  EXPECT_EQ(buyOrder.quantity(), 0);

  auto containsOrder1 = container_.contains(1);
  EXPECT_TRUE(containsOrder1);

  auto order1Itr = container_.find(1);
  EXPECT_EQ(order1Itr->quantity(), 1);
}

TEST_F(RBTreeContainerTestF, CrossFillMultipleOrders) {
  using namespace hermes;
  // insert 5th order at same price level as 2
  container_.insert(Order(5, 2, 1, false, 4));

  auto buyOrder = Order(10, 5, 15, true, 4);
  container_.cross(buyOrder);
  EXPECT_EQ(buyOrder.quantity(), 0);

  // should only contain order with id 4 remaining
  auto containsOrder1 = container_.contains(1);
  auto containsOrder2 = container_.contains(2);
  auto containsOrder3 = container_.contains(3);
  auto containsOrder4 = container_.contains(4);
  auto containsOrder5 = container_.contains(5);
  EXPECT_FALSE(containsOrder1);
  EXPECT_FALSE(containsOrder2);
  EXPECT_FALSE(containsOrder3);
  EXPECT_TRUE(containsOrder4);
  EXPECT_FALSE(containsOrder5);

  auto order4Itr = container_.find(4);
  EXPECT_EQ(order4Itr->quantity(), 4);
}