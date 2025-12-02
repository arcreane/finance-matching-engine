#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <map>
#include <deque>
#include <mutex>
#include <vector>
#include <iostream>
#include "Order.hpp"
#include "Trading.hpp"

// Forward declaration to avoid circular dependency
class MatchingEngine;

/**
 * @class OrderBook
 * @brief Manages the collection and matching of trading orders
 */
class OrderBook {
public:
    /**
     * @brief Bid orders container
     * 
     * Stores buy orders organized by price in descending order.
     * Key: Price, Value: Queue of orders at that price level
     */
    std::map<double, std::deque<Order>, std::greater<double> > bidOrders;

    /**
     * @brief Ask orders container
     * 
     * Stores sell orders organized by price in ascending order.
     * Key: Price, Value: Queue of orders at that price level
     */
    std::map<double, std::deque<Order> > askOrders;

    /**
     * @brief Default constructor
     */
    OrderBook();

    /**
     * @brief Adds a new order to the order book
     *
     * @param order The order to be added to the book
     */
    void addOrder(const Order& order);

    /**
     * @brief Executes the order matching algorithm
     *
     * @return int Number of trades executed in this matching cycle
     */
    int matchOrders();

    /**
     * @brief Displays the current state of the order book
     */
    void displayOrderBook() const;

    /**
     * @brief Displays the history of executed trades
     */
    void displayTrades() const;

    /**
     * @brief Retrieves the most recently executed trade
     *
     * @return const Trade* Pointer to the last trade, or nullptr if no trades
     */
    const Trade* getLastTrade() const;

    /**
     * @brief Sets a reference to the matching engine
     *
     * @param engine Pointer to the MatchingEngine instance
     */
    void setMatchingEngine(MatchingEngine* engine) { matchingEngine = engine; }

private:
    /**
     * @brief Container for all executed trades
     */
    std::vector<Trade> trades;

    /**
     * @brief Tracks the ID for the next trade
     */
    int nextTradeId;

    /**
     * @brief Pointer to the associated MatchingEngine
     */
    MatchingEngine* matchingEngine;

    /**
     * @brief Removes fully executed orders from the order book
     */
    void cleanupExecutedOrders();

    /**
     * @brief Notifies the matching engine about a trade execution
     *
     * @param trade The trade that has been executed
     */
    void notifyMatch(const Trade& trade);

    /**
     * @brief Mutex for thread-safe display operations
     */
    std::mutex displayMutex;
};

#endif // ORDERBOOK_HPP