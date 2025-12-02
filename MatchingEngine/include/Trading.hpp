/**
 * @file Trading.hpp
 * @brief Defines the core Trade structure for the trading system
 *
 * This header provides the fundamental representation of a trade
 * execution in the trading platform, capturing essential transaction details.
 */

#ifndef TRADING_HPP
#define TRADING_HPP

#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>

/**
 * @struct Trade
 * @brief Represents a completed trade transaction
 *
 * Encapsulates all critical information about a single trade execution,
 * including identification, pricing, and temporal details.
 */
struct Trade
{
    /**
     * @brief Unique identifier for the trade
     *
     * Provides a distinct reference for each executed trade
     */
    int tradeId;

    /**
     * @brief Identifier of the buy order involved in the trade
     *
     * References the specific buy order that participated in this trade
     */
    int buyOrderId;

    /**
     * @brief Identifier of the sell order involved in the trade
     *
     * References the specific sell order that participated in this trade
     */
    int sellOrderId;

    /**
     * @brief Market Identification Code (MIC)
     *
     * Identifies the specific market where the trade was executed
     */
    std::string marketIdentificationCode;

    /**
     * @brief Currency used for the trade
     *
     * Specifies the monetary unit in which the trade was conducted
     */
    std::string tradingCurrency;

    /**
     * @brief Execution price of the trade
     *
     * The price at which the buy and sell orders were matched
     */
    double price;

    /**
     * @brief Quantity of financial instruments traded
     *
     * The number of units exchanged in this trade
     */
    int quantity;

    /**
     * @brief Precise timestamp of trade execution
     *
     * Captures the exact moment when the trade was completed
     */
    std::chrono::system_clock::time_point timestamp;

    /**
     * @brief Displays detailed information about the trade
     *
     * Outputs trade details to the console, providing a comprehensive
     * view of the trade's characteristics
     */
    void display() const;
};

// Forward declarations to prevent circular dependencies
class OrderBook;
class MatchingEngine;

#endif // TRADING_HPP
