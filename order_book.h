#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <sstream>
#include <shared_mutex>
#include <iomanip>
#include <mutex>

using namespace std;

enum class OrderType {
    kBuy,
    kSell
};

class Order {
private:
    string order_id;
    double price;
    OrderType order_type;
    int quantity;

public:
    Order(const string& id, double p, OrderType type, int q);

    const string& GetOrderId() const;
    const double& GetPrice() const;
    const OrderType& GetOrderType() const;
    const int& GetQuantity() const;
    void ReduceQuantity(int qty);
};

class OrderBook {
private:
    map<double, queue<Order>, greater<double>> buy_order; // Maintains all buy (bid) orders.
    map<double, queue<Order>> sell_order;// Maintains all sell (ask) orders.
    unordered_map<string, pair<OrderType, double>> order_tracker;// Tracks orders for cancellation.
    mutable std::shared_mutex order_mutex;// Ensures thread-safe access.
    int order_id_counter = 0; // Generates unique order IDs.

    string getCurrentTimestamp(); // Returns the timestamp when a trade is executed.
    void ExecutingTheOrder(); // Executes orders immediately if bid and ask match.
    void AddBuyOrderAndExecute(const Order& order); //Adds a buy order and executes if there's a match.
    void AddSellOrderAndExecute(const Order& order); //Adds a sell order and executes if there's a match.
    void RemoveFromQueue(queue<Order>& order_queue, const string& id); //Removes a specific order by ID.

public:
    void CancelOrder(const string& exchange_id, int id); //// Cancels a specific order by ID.
    void PlaceOrder(const string& exchange_id, OrderType order_type, double price, int quantity); // Places a new order.

    // Helper Methods for Testing
    int GetBuyOrderQuantity(double price) const;   // Returns the quantity of a buy order at a specific price.
    int GetSellOrderQuantity(double price) const; // Returns the quantity of a sell order at a specific price.
    int GetTotalBuyOrders() const;  // Returns the total number of buy orders.
    int GetTotalSellOrders() const; // Returns the total number of sell orders.
};
#endif//ORDER_BOOK_H
