/**
 * @file Order.hpp
 * @brief Defines the Order class and related enumerations for trading system
 *
 * This header provides the core structure for representing trading orders,
 * including various order types, time validity, and essential order attributes.
 */

#ifndef ORDER_HPP
#define ORDER_HPP

#include <chrono>
#include <string>
#include "Instrument.hpp"

/**
 * @enum TimeInForce
 * @brief Specifies the time validity of an order
 *
 * Defines different order duration types in the trading system:
 * - GTD: Good Till Date, order remains valid until a specific expiration date
 * - DAY: Order valid only for the current trading day
 */
enum class TimeInForce
{
    GTD, // Order valid until a specified expiration date
    DAY // Order valid for the current trading session
};

/**
 * @enum OrderType
 * @brief Represents the direction of a trading order
 *
 * Indicates whether the order is intended to buy or sell:
 * - BID: Purchase order
 * - ASK: Sales order
 */
enum class OrderType
{
    BID, // Buy order
    ASK // Sell order
};

/**
 * @enum LimitType
 * @brief Defines the pricing constraint of an order
 *
 * Specifies whether the order has a price limit:
 * - LIMIT: Order with a specific price constraint
 * - NONE: No specific price limitation
 */
enum class LimitType
{
    LIMIT, // Order with a price limit
    NONE // No price limit
};

/**
 * @class Order
 * @brief Represents a comprehensive trading order with all necessary details
 *
 * Encapsulates all critical information for a trading order, including
 * identification, pricing, quantity, and temporal characteristics.
 */
class Order
{
public:
    // Order Identification and Market Attributes
    int idorder; // Unique order identifier
    std::string marketIdentificationCode; // Market Identification Code (MIC)
    std::string tradingCurrency; // Currency used for the trade

    // Timing and Priority Attributes
    std::chrono::system_clock::time_point priority; // Order priority timestamp (nanosecond precision)
    std::chrono::system_clock::time_point expirationDate; // Expiration time for GTD orders

    // Pricing and Quantity Details
    double price; // Order price
    int quantity; // Current order quantity
    int originalqty; // Initial order quantity

    // Order Classification Attributes
    TimeInForce timeinforce; // Order time validity type
    OrderType ordertype; // Order direction (buy/sell)
    LimitType limitType; // Price limitation type

    // Additional Identifiers
    int idinstrument; // Associated instrument identifier
    int idfirm; // Submitting firm identifier

    /**
     * @brief Default constructor
     * 
     * Initializes an empty order with default values
     */
    Order();

    /**
     * @brief Constructor for Good Till Date (GTD) orders
     * 
     * @param idorder Unique order identifier
     * @param marketIdentificationCode Market identifier
     * @param tradingCurrency Currency for the trade
     * @param priority Order priority timestamp
     * @param price Order price
     * @param quantity Current order quantity
     * @param timeinforce Order time validity type
     * @param ordertype Order direction
     * @param limitType Price limitation type
     * @param idinstrument Instrument identifier
     * @param originalqty Initial order quantity
     * @param idfirm Submitting firm identifier
     * @param expirationDate Expiration timestamp for GTD orders
     */
    Order(int idorder, const std::string& marketIdentificationCode, const std::string& tradingCurrency,
          std::chrono::system_clock::time_point priority, double price, int quantity,
          TimeInForce timeinforce, OrderType ordertype, LimitType limitType, int idinstrument, int originalqty,
          int idfirm,
          std::chrono::system_clock::time_point expirationDate);

    /**
     * @brief Constructor for Day orders
     * 
     * @param idorder Unique order identifier
     * @param marketIdentificationCode Market identifier
     * @param tradingCurrency Currency for the trade
     * @param priority Order priority timestamp
     * @param price Order price
     * @param quantity Current order quantity
     * @param timeinforce Order time validity type
     * @param ordertype Order direction
     * @param limitType Price limitation type
     * @param idinstrument Instrument identifier
     * @param originalqty Initial order quantity
     * @param idfirm Submitting firm identifier
     */
    Order(int idorder, const std::string& marketIdentificationCode, const std::string& tradingCurrency,
          std::chrono::system_clock::time_point priority, double price, int quantity,
          TimeInForce timeinforce, OrderType ordertype, LimitType limitType, int idinstrument, int originalqty,
          int idfirm);

    /**
     * @brief Displays detailed information about the order
     * 
     * Outputs order details to the console or logging system
     */
    void display() const;

    /**
     * @brief Validates the order's price against instrument specifications
     * 
     * @param instrument Reference to the instrument for price validation
     * @return bool True if price is valid, false otherwise
     * 
     * Checks if the order price is:
     * - Positive 
     * - Consistent with the instrument's price decimal specification
     */
    bool validatePrice(const Instrument& instrument) const;

    /**
     * @brief Validates the order's quantity against instrument specifications
     * 
     * @param instrument Reference to the instrument for quantity validation
     * @return bool True if quantity is valid, false otherwise
     * 
     * Validates that the order quantity is:
     * - Positive
     * - A multiple of the instrument's lot size
     */
    bool validateQuantity(const Instrument& instrument) const;
};

#endif // ORDER_HPP
