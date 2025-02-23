**Order Book System**

This project implements a **multi-threaded order book system** in C++ using `std::shared_mutex` for thread safety. It supports placing, executing, and canceling limit buy and sell orders, simulating a simplified trading platform used in financial exchanges.

---

Features

- Place **Buy** and **Sell** Orders
- Automatically **Execute Orders** when matching conditions are met
- **Cancel Orders** by ID
- **Display Live Order Book** (buy and sell orders with price and quantity)
- Thread-safe operations using `std::shared_mutex`
- **Unit Testing** with the Google Test Framework

---

**Technologies Used**

- **C++17** (Modern C++ Features)
- **Google Test** (gtest) for unit testing
- **Multithreading** using:
  - `std::shared_mutex` for efficient concurrent access
  - `std::unique_lock` and `std::shared_lock` for read-write operations

---

**Prerequisites**

Before running the project, ensure you have the following installed:

- A **C++17-compatible compiler** (GCC or Clang)
- **Google Test Library** for unit testing
- **CMake** (optional, for build automation)
- **Git** for version control

---

## ⚙️ **Installation & Usage**

### 1️⃣ **Clone the Repository**

```bash
git clone https://github.com/vartika1/OrderBook.git
cd OrderBook
```

### 2️⃣ **Compile the Code**

#### 🔧 To compile the main application:
```bash
g++ -std=c++17 -o order_book_app order_book.cpp main.cpp -pthread
```

To compile and run tests:
```bash
g++ -std=c++17 -o order_book_test order_book.cpp order_book_test.cpp -lgtest -lpthread
./order_book_test
```

---

**Usage Example**

### Running the Application
```bash
./order_book_app
```

### Example Output:
```
Order Placed: EX1-0 | Type: Buy | Price: $100.5 | Quantity: 50 | Time: 2025-02-23 15:00:00
Order Placed: EX1-1 | Type: Sell | Price: $100.0 | Quantity: 30 | Time: 2025-02-23 15:00:01
Executed Trade: 30 shares at price $100.0 at 2025-02-23 15:00:02
```

---

**Running Tests**

To verify the integrity of the system, run the unit tests using Google Test:

```bash
./order_book_test
```

If everything is working correctly, the output should show all tests passing:
```
[==========] Running 5 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 5 tests from OrderBookTest
[ RUN      ] OrderBookTest.PlaceOrder
[       OK ] OrderBookTest.PlaceOrder (0 ms)
[ RUN      ] OrderBookTest.ExecuteOrder
[       OK ] OrderBookTest.ExecuteOrder (0 ms)
[----------] 5 tests from OrderBookTest (0 ms total)
[==========] All tests passed.
```

---

**Project Structure**

```
OrderBook/
│
├── order_book.h           # Order Book header file
├── order_book.cpp         # Core implementation of the order book
├── order_book_test.cpp    # Unit tests using Google Test
├── main.cpp               # Entry point for running the order book system
└── README.md              # Project documentation
```

---

**Author**

Developed by **Vartika Singh**  
[GitHub](https://github.com/vartika1)

---

**License**

This project is open-source and available under the [MIT License](LICENSE).

