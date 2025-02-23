// order_book.cpp
#include "order_book.h"

Order::Order(const string& id, double p, OrderType type, int q)
    : order_id(id), price(p), order_type(type), quantity(q) {}

const string& Order::GetOrderId() const { return order_id; }
const double& Order::GetPrice() const { return price; }
const OrderType& Order::GetOrderType() const { return order_type; }
const int& Order::GetQuantity() const { return quantity; }
void Order::ReduceQuantity(int qty) { quantity -= qty; }

string OrderBook::getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&now_time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void OrderBook::ExecutingTheOrder() {
    if (!buy_order.empty() && !sell_order.empty()) {
        auto best_buy = buy_order.begin();
        auto best_sell = sell_order.begin();

        if (best_buy->first >= best_sell->first) {
            auto& buy_queue = best_buy->second;
            auto& sell_queue = best_sell->second;

            Order& actual_buy_order = buy_queue.front();
            Order& actual_sell_order = sell_queue.front();

            int executed_quantity = min(actual_buy_order.GetQuantity(), actual_sell_order.GetQuantity());
            actual_buy_order.ReduceQuantity(executed_quantity);
            actual_sell_order.ReduceQuantity(executed_quantity);

            cout << "Executed Trade: " << executed_quantity << " shares at price $" << best_sell->first
                 << " at " << getCurrentTimestamp() << endl;

            if (actual_buy_order.GetQuantity() == 0) {
                order_tracker.erase(actual_buy_order.GetOrderId());
                buy_queue.pop();
                if (buy_queue.empty()) buy_order.erase(best_buy);
            }

            if (actual_sell_order.GetQuantity() == 0) {
                order_tracker.erase(actual_sell_order.GetOrderId());
                sell_queue.pop();
                if (sell_queue.empty()) sell_order.erase(best_sell);
            }
        }
    }
}

void OrderBook::AddBuyOrderAndExecute(const Order& order) {
    buy_order[order.GetPrice()].push(order);
    order_tracker[order.GetOrderId()] = { OrderType::kBuy, order.GetPrice() };
    ExecutingTheOrder();
}

void OrderBook::AddSellOrderAndExecute(const Order& order) {
    sell_order[order.GetPrice()].push(order);
    order_tracker[order.GetOrderId()] = { OrderType::kSell, order.GetPrice() };
    ExecutingTheOrder();
}

void OrderBook::RemoveFromQueue(queue<Order>& order_queue, const string& id) {
    queue<Order> temp_queue;
    while (!order_queue.empty()) {
        const Order& front = order_queue.front();
        order_queue.pop();
        if (front.GetOrderId() != id) {
            temp_queue.push(front);
        }
    }
    order_queue = temp_queue;
}

void OrderBook::CancelOrder(const string& exchange_id, int id) {
    unique_lock<shared_mutex> lock(order_mutex);
    string order_id = exchange_id + "-" + to_string(id);
    if (order_tracker.find(order_id) == order_tracker.end()) {
        cout << "Order doesn’t exist" << endl;
        return;
    }

    const pair<OrderType, double>& order_details = order_tracker[order_id];
    if (order_details.first == OrderType::kBuy) {
        queue<Order>& buy_queue = buy_order[order_details.second];
        RemoveFromQueue(buy_queue, order_id);
    } else {
        queue<Order>& sell_queue = sell_order[order_details.second];
        RemoveFromQueue(sell_queue, order_id);
    }

    order_tracker.erase(order_id);
}

void OrderBook::PlaceOrder(const string& exchange_id, OrderType order_type, double price, int quantity) {
     unique_lock<shared_mutex> lock(order_mutex);
    int id = order_id_counter++;
    string order_id = exchange_id + "-" + to_string(id);
    Order new_order(order_id, price, order_type, quantity);

    cout << "Order Placed: " << order_id << " | Type: " << (order_type == OrderType::kBuy ? "Buy" : "Sell")
         << " | Price: $" << price << " | Quantity: " << quantity << " | Time: " << getCurrentTimestamp() << endl;

    if (new_order.GetOrderType() == OrderType::kBuy)
        AddBuyOrderAndExecute(new_order);
    else
        AddSellOrderAndExecute(new_order);
}



//Helper Functions Implementation
int OrderBook::GetBuyOrderQuantity(double price) const {
    shared_lock<shared_mutex> lock(order_mutex);
    auto it = buy_order.find(price);
    if (it != buy_order.end() && !it->second.empty()) {
        return it->second.front().GetQuantity();
    }
    return 0;
}

int OrderBook::GetSellOrderQuantity(double price) const {
    shared_lock<shared_mutex> lock(order_mutex);
    auto it = sell_order.find(price);
    if (it != sell_order.end() && !it->second.empty()) {
        return it->second.front().GetQuantity();
    }
    return 0;
}

int OrderBook::GetTotalBuyOrders() const {
    shared_lock<shared_mutex> lock(order_mutex);
    int total = 0;
    for (const auto& entry : buy_order) {
        total += entry.second.size();
    }
    return total;
}

int OrderBook::GetTotalSellOrders() const {
    shared_lock<shared_mutex> lock(order_mutex);
    int total = 0;
    for (const auto& entry : sell_order) {
        total += entry.second.size();
    }
    return total;
}
