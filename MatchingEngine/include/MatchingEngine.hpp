/**
* @file MatchingEngine.hpp
* @brief Core component of the trading system responsible for order matching and execution
*
* The MatchingEngine class implements a continuous trading system that matches
* orders based on price-time priority, manages trading statistics, and handles
* GTD (Good Till Date) orders.
*/

#ifndef MATCHINGENGINE_HPP
#define MATCHINGENGINE_HPP

#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <iostream>
#include "Trading.hpp"
#include "OrderBook.hpp"
#include "InstrumentManager.hpp"
#include "Order.hpp"

/**
* @class MatchingEngine
* @brief Main trading engine that processes and matches orders
*
* Provides continuous order matching, statistics tracking, and order management
* functionality while running in a separate thread.
*/
class MatchingEngine {
private:
   OrderBook& orderBook;              ///< Reference to the order book
   InstrumentManager& instrumentManager; ///< Reference to the instrument manager
   std::thread engineThread;          ///< Thread for continuous processing
   std::atomic<bool> isRunning;       ///< Engine status flag

   /**
    * @brief Trading statistics structure
    *
    * Holds atomic counters and values for various trading metrics
    */
   struct {
       std::atomic<int> dailyTradeCount{0};    ///< Number of trades for current day
       std::atomic<double> dailyVolume{0.0};   ///< Trading volume for current day
       std::chrono::system_clock::time_point lastReset; ///< Last statistics reset time
       std::atomic<int> totalTradeCount{0};    ///< Total number of trades
       std::atomic<double> totalVolume{0.0};   ///< Total trading volume
       std::atomic<int> matchingAttempts{0};   ///< Number of matching attempts
       std::atomic<int> successfulMatches{0};  ///< Number of successful matches
   } stats;

   std::mutex displayMutex;           ///< Mutex for thread-safe display operations

   /**
    * @brief Main processing loop of the matching engine
    *
    * Continuously processes orders, checks GTD orders, and updates statistics
    */
   void run();

   /**
    * @brief Checks and removes expired GTD orders
    */
   void checkGTDOrders();

   /**
    * @brief Resets daily trading statistics
    */
   void resetDailyStats();

public:
   /**
    * @brief Constructs a new MatchingEngine
    *
    * @param ob Reference to the order book
    * @param im Reference to the instrument manager
    */
   MatchingEngine(OrderBook& ob, InstrumentManager& im);

   /**
    * @brief Destructor ensures proper engine shutdown
    */
   ~MatchingEngine();

   /**
    * @brief Starts the matching engine
    *
    * Initializes the processing thread and begins order matching
    */
   void start();

   /**
    * @brief Stops the matching engine
    *
    * Ensures proper shutdown of the processing thread
    */
   void stop();

   /**
    * @brief Checks if the engine is currently running
    *
    * @return true if the engine is running
    * @return false if the engine is stopped
    */
   bool isEngineRunning() const;

   /**
    * @brief Displays current engine status and basic statistics
    */
   void displayEngineStatus() const;

   /**
    * @brief Displays detailed trading statistics
    */
   void displayDetailedStats() const;

   /**
    * @brief Displays all active GTD orders
    */
   void displayGTDOrders() const;

   /**
    * @brief Displays available commands and their descriptions
    */
   void displayHelp() const;

   /**
    * @brief Validates and adds a new order to the system
    *
    * @param order The order to be added
    * @return true if the order was successfully added
    * @return false if the order validation failed
    */
   bool addAndValidateOrder(const Order& order);

   /**
    * @brief Updates trading statistics after a trade
    *
    * @param trade The executed trade to record
    */
   void updateStats(const Trade& trade);
};

#endif // MATCHINGENGINE_HPP