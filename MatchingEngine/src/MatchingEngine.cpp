/**
 * @file MatchingEngine.cpp
 * @brief Implementation of the MatchingEngine class methods
 *
 * Provides the core functionality for continuous trading, 
 * order matching, and system performance tracking.
 */

#include "MatchingEngine.hpp"
#include "Order.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

/**
 * @brief Constructs a MatchingEngine instance
 *
 * @param ob Reference to the OrderBook for managing orders
 * @param im Reference to the InstrumentManager for tracking instruments
 *
 * Initializes the matching engine with default statistics and 
 * sets up the connection with the order book.
 */
MatchingEngine::MatchingEngine(OrderBook& ob, InstrumentManager& im)
    : orderBook(ob), instrumentManager(im), isRunning(false)
{
    // Initialize all statistical counters to zero
    stats.dailyTradeCount.store(0);
    stats.dailyVolume.store(0.0);
    stats.lastReset = std::chrono::system_clock::now();
    stats.totalTradeCount.store(0);
    stats.totalVolume.store(0.0);
    stats.matchingAttempts.store(0);
    stats.successfulMatches.store(0);
    orderBook.setMatchingEngine(this);
}

/**
 * @brief Destructor ensures proper shutdown of the matching engine
 */
MatchingEngine::~MatchingEngine()
{
    stop();
}

/**
 * @brief Main processing loop for the matching engine
 *
 * Continuously performs these key operations:
 * - Attempt order matching
 * - Check and remove expired GTD orders
 * - Reset daily statistics
 * - Provide periodic status updates
 */
void MatchingEngine::run()
{
    auto lastStatusUpdate = std::chrono::system_clock::now();
    auto lastGTDCheck = std::chrono::system_clock::now();
    auto lastStatsReset = std::chrono::system_clock::now();

    while (isRunning)
    {
        try
        {
            auto now = std::chrono::system_clock::now();
            auto now_time_t = std::chrono::system_clock::to_time_t(now);

            // Daily statistics reset at midnight
            if (now - lastStatsReset > std::chrono::hours(24))
            {
                std::lock_guard<std::mutex> lock(displayMutex);
                resetDailyStats();
                lastStatsReset = now;
                std::cout << "\nDaily statistics reset at "
                    << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S") << std::endl;
            }

            // Attempt order matching
            stats.matchingAttempts.fetch_add(1, std::memory_order_relaxed);
            int matches = orderBook.matchOrders();
            if (matches > 0)
            {
                std::lock_guard<std::mutex> lock(displayMutex);
                stats.successfulMatches.fetch_add(matches, std::memory_order_relaxed);
                std::cout << "\nMatched " << matches << " orders at "
                    << std::put_time(std::localtime(&now_time_t), "%H:%M:%S") << std::endl;
            }

            // Periodic GTD order check
            if (now - lastGTDCheck > std::chrono::hours(1))
            {
                std::lock_guard<std::mutex> lock(displayMutex);
                std::cout << "\nChecking GTD orders at "
                    << std::put_time(std::localtime(&now_time_t), "%H:%M:%S") << std::endl;
                checkGTDOrders();
                displayGTDOrders();
                lastGTDCheck = now;
            }

            // Periodic status update
            if (now - lastStatusUpdate > std::chrono::seconds(30))
            {
                std::lock_guard<std::mutex> lock(displayMutex);
                std::cout << "\n=== Periodic Status Update at "
                    << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S")
                    << " ===\n";
                displayEngineStatus();
                lastStatusUpdate = now;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        catch (const std::exception& e)
        {
            std::cerr << "\nError in trading engine: " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Starts the matching engine in a separate thread
 *
 * Initializes all statistical counters and launches 
 * the continuous trading process.
 */
void MatchingEngine::start()
{
    if (!isRunning)
    {
        isRunning = true;

        // Reset all statistical counters
        stats.dailyTradeCount.store(0, std::memory_order_relaxed);
        stats.dailyVolume.store(0.0, std::memory_order_relaxed);
        stats.totalTradeCount.store(0, std::memory_order_relaxed);
        stats.totalVolume.store(0.0, std::memory_order_relaxed);
        stats.matchingAttempts.store(0, std::memory_order_relaxed);
        stats.successfulMatches.store(0, std::memory_order_relaxed);
        stats.lastReset = std::chrono::system_clock::now();

        // Launch processing thread
        engineThread = std::thread(&MatchingEngine::run, this);
        std::cout << "Trading Engine started in continuous mode." << std::endl;
    }
}

/**
 * @brief Stops the matching engine and waits for the processing thread to finish
 */
void MatchingEngine::stop()
{
    if (isRunning)
    {
        isRunning = false;
        if (engineThread.joinable())
        {
            engineThread.join();
        }
        std::cout << "Trading Engine stopped." << std::endl;
    }
}

/**
 * @brief Checks if the matching engine is currently running
 *
 * @return bool True if the engine is active, false otherwise
 */
bool MatchingEngine::isEngineRunning() const
{
    return isRunning;
}

/**
 * @brief Displays the current status of the trading engine
 *
 * Outputs key performance metrics, system state, and 
 * current trading book statistics.
 */
void MatchingEngine::displayEngineStatus() const
{
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    std::cout << "\n=== Trading Engine Status ===\n";
    std::cout << "Time: " << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S") << "\n";
    std::cout << "Engine Status: " << (isRunning ? "Running" : "Stopped") << "\n";
    std::cout << "Trading Statistics:\n";
    std::cout << "  - Daily Trades: " << stats.dailyTradeCount << "\n";
    std::cout << "  - Daily Volume: " << std::fixed << std::setprecision(2) << stats.dailyVolume << "\n";
    std::cout << "  - Total Trades: " << stats.totalTradeCount << "\n";
    std::cout << "System Status:\n";
    std::cout << "  - Instruments: " << instrumentManager.getInstruments().size() << "\n";
    std::cout << "  - BID Levels: " << orderBook.bidOrders.size() << "\n";
    std::cout << "  - ASK Levels: " << orderBook.askOrders.size() << "\n";
    std::cout << "==========================\n\n";
}

/**
 * @brief Displays comprehensive trading performance statistics
 *
 * Provides detailed insights into daily and total trading performance,
 * including trade counts, volumes, and matching efficiency.
 */
void MatchingEngine::displayDetailedStats() const
{
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    std::cout << "\n=== Detailed Trading Statistics ===\n";
    std::cout << "Current Time: " << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S") << "\n";
    std::cout << "Daily Performance:\n";
    std::cout << "  - Trades Today: " << stats.dailyTradeCount << "\n";
    std::cout << "  - Daily Volume: " << std::fixed << std::setprecision(2) << stats.dailyVolume << "\n";
    std::cout << "Total Performance:\n";
    std::cout << "  - Total Trades: " << stats.totalTradeCount << "\n";
    std::cout << "  - Total Volume: " << stats.totalVolume << "\n";
    std::cout << "Engine Metrics:\n";
    std::cout << "  - Matching Attempts: " << stats.matchingAttempts << "\n";
    std::cout << "  - Successful Matches: " << stats.successfulMatches << "\n";
    std::cout << "  - Success Rate: "
        << (stats.matchingAttempts > 0 ? (100.0 * stats.successfulMatches / stats.matchingAttempts) : 0)
        << "%\n";
    std::cout << "=============================\n";
}

/**
 * @brief Displays all current Good Till Date (GTD) orders
 *
 * Lists active GTD orders for both BID and ASK sides, 
 * showing their expiration times.
 */
void MatchingEngine::displayGTDOrders() const
{
    auto now = std::chrono::system_clock::now();
    bool hasGTDOrders = false;

    std::cout << "\n=== GTD Orders Status ===\n";

    // Scan BID orders
    for (const auto& [price, orders] : orderBook.bidOrders)
    {
        for (const auto& order : orders)
        {
            if (order.timeinforce == TimeInForce::GTD)
            {
                hasGTDOrders = true;
                auto timeToExpiry = std::chrono::duration_cast<std::chrono::hours>(
                    order.expirationDate - now).count();
                std::cout << "BID Order " << order.idorder
                    << " (Price: " << order.price
                    << ", Qty: " << order.quantity
                    << ") expires in " << timeToExpiry << " hours\n";
            }
        }
    }

    // Scan ASK orders
    for (const auto& [price, orders] : orderBook.askOrders)
    {
        for (const auto& order : orders)
        {
            if (order.timeinforce == TimeInForce::GTD)
            {
                hasGTDOrders = true;
                auto timeToExpiry = std::chrono::duration_cast<std::chrono::hours>(
                    order.expirationDate - now).count();
                std::cout << "ASK Order " << order.idorder
                    << " (Price: " << order.price
                    << ", Qty: " << order.quantity
                    << ") expires in " << timeToExpiry << " hours\n";
            }
        }
    }

    if (!hasGTDOrders)
    {
        std::cout << "No GTD orders currently in the book.\n";
    }
    std::cout << "========================\n";
}

/**
 * @brief Displays available commands for the trading engine
 *
 * Provides a help menu showing all interactive commands 
 * and their descriptions.
 */
void MatchingEngine::displayHelp() const
{
    std::cout << "\n=== Trading Engine Commands ===\n";
    std::cout << "Available commands:\n";
    std::cout << "  status   - Display current engine status\n";
    std::cout << "  stats    - Display detailed trading statistics\n";
    std::cout << "  gtd      - Display GTD orders and their expiry\n";
    std::cout << "  order    - Add a new test order\n";
    std::cout << "  display  - Show complete order book and trades\n";
    std::cout << "  help     - Display this help message\n";
    std::cout << "  quit     - Stop the engine and exit\n";
    std::cout << "============================\n";
}

/**
 * @brief Removes expired Good Till Date (GTD) orders
 *
 * Scans through BID and ASK orders, removing those that have 
 * passed their expiration date.
 */
void MatchingEngine::checkGTDOrders()
{
    auto now = std::chrono::system_clock::now();
    int expiredOrders = 0;

    // Remove expired BID orders
    for (auto& [price, orders] : orderBook.bidOrders)
    {
        orders.erase(
            std::remove_if(orders.begin(), orders.end(),
                           [now, &expiredOrders](const Order& order)
                           {
                               if (order.timeinforce == TimeInForce::GTD && order.expirationDate <= now)
                               {
                                   std::cout << "Removing expired GTD order ID: " << order.idorder << std::endl;
                                   expiredOrders++;
                                   return true;
                               }
                               return false;
                           }),
            orders.end()
        );
    }

    // Remove expired ASK orders
    for (auto& [price, orders] : orderBook.askOrders)
    {
        orders.erase(
            std::remove_if(orders.begin(), orders.end(),
                           [now, &expiredOrders](const Order& order)
                           {
                               if (order.timeinforce == TimeInForce::GTD && order.expirationDate <= now)
                               {
                                   std::cout << "Removing expired GTD order ID: " << order.idorder << std::endl;
                                   expiredOrders++;
                                   return true;
                               }
                               return false;
                           }),
            orders.end()
        );
    }

    if (expiredOrders > 0)
    {
        std::cout << "Removed " << expiredOrders << " expired GTD orders\n";
    }
}

/**
 * @brief Resets daily trading statistics
 *
 * Zeroes out daily-specific performance metrics 
 * and updates the reset timestamp.
 */
void MatchingEngine::resetDailyStats()
{
    stats.dailyTradeCount = 0;
    stats.dailyVolume = 0.0;
    stats.matchingAttempts = 0;
    stats.successfulMatches = 0;
    stats.lastReset = std::chrono::system_clock::now();
}

/**
 * @brief Updates trading statistics after a trade execution
 *
 * @param trade The most recently completed trade
 *
 * Incrementally updates daily and total trade counts, 
 * volumes, and match statistics.
 */
void MatchingEngine::updateStats(const Trade& trade)
{
    double tradeValue = trade.quantity * trade.price;
    stats.dailyTradeCount.fetch_add(1, std::memory_order_relaxed);
    double currentDailyVolume = stats.dailyVolume.load(std::memory_order_relaxed);
    stats.dailyVolume.store(currentDailyVolume + tradeValue, std::memory_order_relaxed);

    stats.totalTradeCount.fetch_add(1, std::memory_order_relaxed);
    double currentTotalVolume = stats.totalVolume.load(std::memory_order_relaxed);
    stats.totalVolume.store(currentTotalVolume + tradeValue, std::memory_order_relaxed);

    stats.successfulMatches.fetch_add(1, std::memory_order_relaxed);
}

/**
 * @brief Validates and adds a new order to the trading system
 *
 * @param order The order to be added and validated
 * @return bool True if the order was successfully added, false otherwise
 *
 * Performs comprehensive order validation by:
 * - Matching the order with a registered instrument
 * - Checking price and quantity against instrument specifications
 * - Adding the order to the order book
 * - Attempting immediate order matching
 */
bool MatchingEngine::addAndValidateOrder(const Order& order)
{
    // Find matching instrument for the order
    for (const auto& instrument : instrumentManager.getInstruments())
    {
        if (instrument.idinstrument == order.idinstrument &&
            instrument.marketIdentificationCode == order.marketIdentificationCode &&
            instrument.tradingCurrency == order.tradingCurrency)
        {
            // Validate order price and quantity
            if (order.validatePrice(instrument) && order.validateQuantity(instrument))
            {
                // Add order to the order book
                orderBook.addOrder(order);
                std::cout << "Order added - ID: " << order.idorder
                    << " Type: " << (order.ordertype == OrderType::BID ? "BID" : "ASK")
                    << " Price: " << std::fixed << std::setprecision(2) << order.price
                    << " Quantity: " << order.quantity << "\n";

                // Attempt immediate order matching
                int matches = orderBook.matchOrders();
                if (matches > 0)
                {
                    const Trade* lastTrade = orderBook.getLastTrade();
                    if (lastTrade)
                    {
                        updateStats(*lastTrade);
                    }
                }
                return true;
            }
            std::cout << "Order validation failed\n";
            break;
        }
    }
    std::cout << "No matching instrument found for order\n";
    return false;
}
