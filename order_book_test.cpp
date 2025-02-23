#include <gtest/gtest.h>
#include "order_book.h"

class OrderBookTest : public ::testing::Test {
protected:
    OrderBook ob;

    void SetUp() override {}
    void TearDown() override {}
};

//Test Order Placement Verification
TEST_F(OrderBookTest, PlaceOrder) {
    ob.PlaceOrder("EX1", OrderType::kBuy, 100.5, 50); // Buy order
    ob.PlaceOrder("EX1", OrderType::kSell, 101.0, 30); // Sell order (no execution)

    // ✅ Verify buy and sell order quantities using helper functions
    ASSERT_EQ(ob.GetBuyOrderQuantity(100.5), 50);
     ASSERT_EQ(ob.GetSellOrderQuantity(101.0), 30);
    ASSERT_EQ(ob.GetTotalBuyOrders(), 1);
    ASSERT_EQ(ob.GetTotalSellOrders(), 1);
}

//Test Order Execution Verification
TEST_F(OrderBookTest, ExecuteOrder) {
    ob.PlaceOrder("EX1", OrderType::kBuy, 100.5, 50);
    ob.PlaceOrder("EX1", OrderType::kSell, 100.0, 30); // Should execute immediately

    //Verify buy order remains with 20 quantity after execution
    ASSERT_EQ(ob.GetBuyOrderQuantity(100.5), 20);
    //Verify sell order was executed and removed from order book
    ASSERT_EQ(ob.GetSellOrderQuantity(100.0), 0);
    ASSERT_EQ(ob.GetTotalSellOrders(), 0);
      ASSERT_EQ(ob.GetTotalBuyOrders(), 1);
}

// Test Cancellation of Non-Existent Order
TEST_F(OrderBookTest, CancelNonExistentOrder) {
    testing::internal::CaptureStdout();
    ob.CancelOrder("EX1", 999); // Non-existent order
    std::string output = testing::internal::GetCapturedStdout();

    // Verify cancellation attempt results in appropriate message
    ASSERT_NE(output.find("Order doesn’t exist"), std::string::npos);
}


// Full Order Lifecycle with Execution and Cancellation
TEST_F(OrderBookTest, FullOrderLifecycle) {
    ob.PlaceOrder("EX1", OrderType::kBuy, 100.5, 50);
    ob.PlaceOrder("EX1", OrderType::kSell, 100.0, 30);
    ob.CancelOrder("EX1", 0); // Attempt to cancel executed order

    // Verify remaining quantity after execution
    ASSERT_NE(ob.GetBuyOrderQuantity(100.5), 20);
    // Verify no sell orders remain
    ASSERT_NE(ob.GetTotalSellOrders(), 1);
}

// ✅ Test Multiple Orders and Verify Remaining Quantities
TEST_F(OrderBookTest, MultipleOrders) {
    ob.PlaceOrder("EX1", OrderType::kBuy, 101.0, 40);
    ob.PlaceOrder("EX1", OrderType::kSell, 100.0, 20);
    ob.PlaceOrder("EX1", OrderType::kSell, 101.0, 10); // Should execute against buy order

    // ✅ Verify remaining quantities after execution
    ASSERT_EQ(ob.GetBuyOrderQuantity(101.0), 10);
    ASSERT_EQ(ob.GetSellOrderQuantity(101.0), 0);
    ASSERT_EQ(ob.GetTotalBuyOrders(), 1);
    ASSERT_EQ(ob.GetTotalSellOrders(), 0);
}

// ✅ Entry point for running all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
