#include <gtest/gtest.h>

#include <memory>

#include "ContainerTest.hxx"
#include "OrderTest.hxx"

#include "../../src/orderbook/Container.hxx"
#include "../../src/orderbook/Order.hxx"
#include "../../src/orderbook/Orderbook.hxx"

class RBTreeContainerTestF : public ::testing::Test {
public:
  RBTreeContainerTestF() : container_{} {}

  hermes::RBTreeContainer<hermes::Order> container_;

  void SetUp() override {
    using namespace hermes;
    container_.insert(Order(1, 2, 3, true, 4));
    container_.insert(Order(2, 3, 4, true, 4));
    container_.insert(Order(3, 4, 5, true, 4));
    container_.insert(Order(4, 5, 6, true, 4));
  }

  void TearDown() override {}
};

TEST(ContainerConcept, TestContainerWorks) {
  using namespace hermes;
  TestOrder order;
  TestContainer<TestOrder> orderContainer;
  orderContainer.insert(order);
  orderContainer.modify(order);
  orderContainer.trade(order);
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
  ASSERT_EQ(order.id(), 2);
}

TEST_F(RBTreeContainerTestF, ModifyOrder) {
  using namespace hermes;
  auto id = 2;
  auto exchId = 4;
  auto newPrice = 100000;
  auto newQuantity = 20014;
  auto newOrder = Order(id, newPrice, newQuantity, true, exchId);
  container_.modify(std::move(newOrder));
  auto findRes = container_.find(id);
  ASSERT_EQ(findRes.id(), id);
  ASSERT_EQ(findRes.price(), newPrice);
  ASSERT_EQ(findRes.quantity(), newQuantity);
  ASSERT_EQ(findRes.exchangeId(), exchId);
}

TEST_F(RBTreeContainerTestF, BestPrice) {
  ASSERT_EQ(container_.bestPrice(), 2);
}