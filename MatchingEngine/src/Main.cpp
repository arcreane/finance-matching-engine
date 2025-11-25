/**
 * @file main.cpp
 * @brief Demonstration and testing of the Euronext Trading Engine
 *
 * This application showcases the core functionalities of a trading system,
 * including instrument management, order book operations, and continuous trading.
 */

#include <iostream>
#include <set>
#include <chrono>
#include <random>
#include <thread>
#include "../include/Instrument.hpp"
#include "../include/Order.hpp"
#include "../include/OrderBook.hpp"
#include "../include/Utils.hpp"
#include "../include/InstrumentManager.hpp"
#include "../include/MatchingEngine.hpp"
#include <QApplication>
#include "MainWindow.h"

/**
 * @brief Utility template to get the number of enum values
 *
 * Provides a compile-time count of enum values for random generation.
 * Specialized for specific enums used in the trading system.
 *
 * @tparam T Enum type to count
 * @return int Number of enum values
 */
template <typename T>
int enumCount();

// Specializations for specific enums
template <>
int enumCount<TimeInForce>() { return 2; }

template <>
int enumCount<OrderType>() { return 2; }

template <>
int enumCount<LimitType>() { return 2; }

/**
 * @brief Generates a random enum value
 *
 * Uses a random device and uniform distribution to select
 * a random enum value from the available options.
 *
 * @tparam T Enum type to generate
 * @return T Randomly selected enum value
 */
template <typename T>
T getRandomEnumValue()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, enumCount<T>() - 1);
    return static_cast<T>(dis(gen));
}

/**
 * @brief Main entry point for the Euronext Trading Engine demonstration
 *
 * Demonstrates key features of the trading system:
 * 1. System Initialization
 * 2. Instrument Management
 * 3. Price-Time Priority Algorithm
 * 4. Continuous Trading Mode
 * 5. Interactive Command Interface
 *
 * @return int Program exit status
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(1000, 600);
    w.show();
    return a.exec();
    /* std::cout << "=== EURONEXT TRADING ENGINE DEMONSTRATION ===\n\n";

    // System Initialization
    std::cout << "=== System Initialization ===\n";
    OrderBook orderBook;
    InstrumentManager instrumentManager;

    // Demonstrate instrument management with uniqueness checks
    std::cout << "Testing Instrument Management:\n";
    instrumentManager.addInstrument(Instrument(1, "XPAR", "EUR", "AAPL", 20220101, State::ACTIVE,
                                               150, 1001, 100, 2, 1, 1, 2022));
    instrumentManager.addInstrument(Instrument(2, "XPAR", "EUR", "MSFT", 20220102, State::ACTIVE,
                                               2740.01, 1002, 400, 2, 2, 2, 2023));

    // Test duplicate instrument rejection
    std::cout << "\nTesting duplicate rejection:\n";
    instrumentManager.addInstrument(Instrument(1, "XPAR", "EUR", "AAPL", 20220101, State::ACTIVE,
                                               150, 1001, 100, 2, 1, 1, 2022));

    // Test adding an instrument with different parameters
    instrumentManager.addInstrument(Instrument(3, "XPAR", "GBP", "NVDI", 20220101, State::ACTIVE,
                                               150, 1001, 100, 2, 1, 1, 2022));

    // Retrieve and display registered instruments
    const auto& instruments = instrumentManager.getInstruments();
    std::cout << "\nTotal instruments registered: " << instruments.size() << "\n\n";

    // Price-Time Priority (PTP) Algorithm
    std::cout << "\n=== Price-Time Priority (PTP) Algorithm ===\n";
    auto now = std::chrono::system_clock::now();
    Instrument testInstrument = instruments[0];

    std::cout << "\nCreating test orders with various prices and timestamps:\n";

    // Test 1: Basic Price-Time Priority matching
    std::cout << "\nTest 1 - Basic PTP matching:\n";
    Order bidHighPrice(1001, "XPAR", "EUR", now, 155.0, 300,
                       TimeInForce::DAY, OrderType::BID, LimitType::LIMIT, 1, 300, 2001);
    Order askLowPrice(2001, "XPAR", "EUR", now + std::chrono::milliseconds(100),
                      148.0, 200, TimeInForce::DAY, OrderType::ASK, LimitType::LIMIT, 1, 200, 3001);

    orderBook.addOrder(bidHighPrice);
    orderBook.addOrder(askLowPrice);

    // Test 2: Time Priority at same price point
    std::cout << "\nTest 2 - Time Priority at same price:\n";
    Order bidSamePrice1(1002, "XPAR", "EUR", now + std::chrono::milliseconds(200),
                        155.0, 200, TimeInForce::DAY, OrderType::BID, LimitType::LIMIT, 1, 200, 2002);
    Order bidSamePrice2(1003, "XPAR", "EUR", now + std::chrono::milliseconds(300),
                        155.0, 200, TimeInForce::DAY, OrderType::BID, LimitType::LIMIT, 1, 200, 2003);

    orderBook.addOrder(bidSamePrice1);
    orderBook.addOrder(bidSamePrice2);

    // Test 3: Good Till Date (GTD) Orders
    std::cout << "\nTest 3 - GTD Orders:\n";
    Order gtdOrder(3001, "XPAR", "EUR", now, 152.0, 100,
                   TimeInForce::GTD, OrderType::ASK, LimitType::LIMIT, 1, 100, 4001,
                   now + std::chrono::hours(24));

    orderBook.addOrder(gtdOrder);

    std::cout << "\nOrderBook after initial orders:\n";
    orderBook.displayOrderBook();

    // Continuous Trading Mode
    std::cout << "\n=== Continuous Trading Mode ===\n";
    std::cout << "\nStarting Trading Engine...\n";
    MatchingEngine engine(orderBook, instrumentManager);
    engine.start();

    // Wait for initial matches to be processed
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Display help menu and start interactive interface
    engine.displayHelp();
    std::cout << "\n";

    // Interactive command processing loop
    std::string command;
    while (true)
    {
        std::cout << "Command > ";
        std::getline(std::cin, command);

        // Command handling logic
        if (command == "status")
        {
            engine.displayEngineStatus();
        }
        else if (command == "stats")
        {
            engine.displayDetailedStats();
        }
        else if (command == "gtd")
        {
            engine.displayGTDOrders();
        }
        else if (command == "help")
        {
            engine.displayHelp();
        }
        else if (command == "order")
        {
            static bool isBid = true;
            static bool isGTD = false;
            static int orderId = 5000;

            auto orderTime = std::chrono::system_clock::now();

            double orderPrice;
            if (isBid)
            {
                orderPrice = isGTD ? 145.0 : 155.0;
            }
            else
            {
                orderPrice = isGTD ? 160.0 : 148.0;
            }

            Order newOrder(orderId++, "XPAR", "EUR",
                           orderTime,
                           orderPrice,
                           100,
                           isGTD ? TimeInForce::GTD : TimeInForce::DAY,
                           isBid ? OrderType::BID : OrderType::ASK,
                           LimitType::LIMIT,
                           1, 100, 1001,
                           isGTD ? orderTime + std::chrono::hours(24) : orderTime
            );

            if (engine.addAndValidateOrder(newOrder))
            {
                std::cout << "Added new " << (isBid ? "BID" : "ASK")
                    << " order (TimeInForce: " << (isGTD ? "GTD" : "DAY")
                    << ", Price: " << orderPrice << ")\n";
                isBid = !isBid;
                isGTD = !isGTD;
            }
        }
        else if (command == "display")
        {
            orderBook.displayOrderBook();
            orderBook.displayTrades();
        }
        else if (command == "quit")
        {
            std::cout << "Stopping Trading Engine...\n";
            engine.stop();
            break;
        }
        else
        {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

    std::cout << "\n=== END OF DEMONSTRATION ===\n";
    */
}
