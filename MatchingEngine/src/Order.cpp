/**
 * @file Order.cpp
 * @brief Implementation of the Order class methods
 *
 * Provides core functionality for creating, validating, 
 * and displaying trading orders.
 */

#include "Order.hpp"
#include "Instrument.hpp"

#include <cmath>
#include <iomanip>
#include <ctime>
#include <iostream>

/**
 * @brief Default constructor for Order
 *
 * Initializes an order with default/zero values, 
 * representing an empty or uninitialized order.
 */
Order::Order()
    : idorder(0),
      marketIdentificationCode(""),
      tradingCurrency(""),
      priority(std::chrono::system_clock::now()),
      price(0.0),
      quantity(0),
      timeinforce(TimeInForce::DAY),
      ordertype(OrderType::BID),
      limitType(LimitType::LIMIT),
      idinstrument(0),
      originalqty(0),
      idfirm(0),
      expirationDate(std::chrono::system_clock::now())
{
}

/**
 * @brief Constructor for Good Till Date (GTD) orders
 *
 * @param idorder Unique order identifier
 * @param marketIdentificationCode Market Identification Code
 * @param tradingCurrency Currency for the trade
 * @param priority Order priority timestamp
 * @param price Order price
 * @param quantity Order quantity
 * @param timeinforce Time validity type
 * @param ordertype Order direction (BID/ASK)
 * @param limitType Price limitation type
 * @param idinstrument Instrument identifier
 * @param originalqty Initial order quantity
 * @param idfirm Firm identifier
 * @param expirationDate Expiration timestamp for GTD orders
 */
Order::Order(int idorder, const std::string& marketIdentificationCode, const std::string& tradingCurrency,
             std::chrono::system_clock::time_point priority, double price, int quantity,
             TimeInForce timeinforce, OrderType ordertype, LimitType limitType, int idinstrument, int originalqty,
             int idfirm, std::chrono::system_clock::time_point expirationDate)
    : idorder(idorder),
      marketIdentificationCode(marketIdentificationCode),
      tradingCurrency(tradingCurrency),
      priority(priority),
      price(price),
      quantity(quantity),
      timeinforce(timeinforce),
      ordertype(ordertype),
      limitType(limitType),
      idinstrument(idinstrument),
      originalqty(originalqty),
      idfirm(idfirm),
      expirationDate(expirationDate)
{
}

/**
 * @brief Constructor for Day orders
 *
 * @param idorder Unique order identifier
 * @param marketIdentificationCode Market Identification Code
 * @param tradingCurrency Currency for the trade
 * @param priority Order priority timestamp
 * @param price Order price
 * @param quantity Order quantity
 * @param timeinforce Time validity type (always DAY)
 * @param ordertype Order direction (BID/ASK)
 * @param limitType Price limitation type
 * @param idinstrument Instrument identifier
 * @param originalqty Initial order quantity
 * @param idfirm Firm identifier
 *
 * Creates a day order with an empty/null expiration timestamp
 */
Order::Order(int idorder, const std::string& marketIdentificationCode, const std::string& tradingCurrency,
             std::chrono::system_clock::time_point priority, double price, int quantity,
             TimeInForce timeinforce, OrderType ordertype, LimitType limitType, int idinstrument, int originalqty,
             int idfirm)
    : idorder(idorder),
      marketIdentificationCode(marketIdentificationCode),
      tradingCurrency(tradingCurrency),
      priority(priority),
      price(price),
      quantity(quantity),
      timeinforce(timeinforce),
      ordertype(ordertype),
      limitType(limitType),
      idinstrument(idinstrument),
      originalqty(originalqty),
      idfirm(idfirm)
{
    // Set expiration date to an empty time point for DAY orders
    expirationDate = std::chrono::system_clock::time_point{};
}

/**
 * @brief Displays comprehensive details of the order
 *
 * Outputs all order attributes to the console in a 
 * human-readable, structured format.
 */
void Order::display() const
{
    std::time_t priority_time = std::chrono::system_clock::to_time_t(priority);
    std::cout << "====== Order Information =======\n";
    std::cout << "Order ID: " << idorder << "\n";
    std::cout << "Market Identification Code (MIC): " << marketIdentificationCode << "\n";
    std::cout << "Trading Currency: " << tradingCurrency << "\n";
    std::cout << "Priority: " << std::ctime(&priority_time);

    // Extract and display nanosecond precision
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(priority.time_since_epoch()).count() % 1000000000;
    std::cout << "Priority Nanoseconds: " << ns << "\n";

    std::cout << "Price: " << price << "\n";
    std::cout << "Quantity: " << quantity << "\n";
    std::cout << "Time In Force: " << (timeinforce == TimeInForce::GTD ? "GTD" : "DAY") << "\n";
    std::cout << "Order Type: " << (ordertype == OrderType::BID ? "BID" : "ASK") << "\n";
    std::cout << "Instrument ID: " << idinstrument << "\n";
    std::cout << "Original Quantity: " << originalqty << "\n";
    std::cout << "Firm ID: " << idfirm << "\n";

    // Display expiration date only for GTD orders
    if (timeinforce == TimeInForce::GTD)
    {
        std::time_t expTime = std::chrono::system_clock::to_time_t(expirationDate);
        std::cout << "Expiration Date: " << std::ctime(&expTime);
    }
    else
    {
        std::cout << "Expiration Date: N/A (DAY order)\n";
    }

    std::cout << "================================\n";
}

/**
 * @brief Validates the order's price against instrument specifications
 *
 * @param instrument Reference instrument for validation
 * @return bool True if price is valid, false otherwise
 *
 * Checks price validity by ensuring:
 * 1. Price is strictly positive
 * 2. Price is a multiple of the instrument's price decimal precision
 */
bool Order::validatePrice(const Instrument& instrument) const
{
    // Check for positive price
    if (price <= 0)
    {
        std::cout << "ERROR: Price must be strictly positive.\n";
        return false;
    }

    // 1. Calculate the factor (e.g., 100 for 2 decimals)
    double precisionFactor = std::pow(10, instrument.pricedecimal);

    // 2. Multiply the price by the factor
    double multipliedPrice = price * precisionFactor;

    // 3. Round the result to the nearest integer
    double roundedPrice = std::round(multipliedPrice);

    // 4. Check if the multiplied price is close to the rounded integer within tolerance
    // This confirms that the original price was a valid multiple of the precision.
    double tolerance = 1e-8;

    if (std::fabs(multipliedPrice - roundedPrice) > tolerance)
    {
        std::cout << "ERROR: Price (" << price << ") must be a multiple of the instrument's pricedecimal (" << instrument.pricedecimal << ").\n";
        // Optional debug output:
        // std::cout << "DEBUG: Multiplied: " << multipliedPrice << ", Rounded: " << roundedPrice << ", Difference: " << std::fabs(multipliedPrice - roundedPrice) << "\n";
        return false;
    }

    return true;
}

/**
 * @brief Validates the order's quantity against instrument specifications
 *
 * @param instrument Reference instrument for validation
 * @return bool True if quantity is valid, false otherwise
 *
 * Performs quantity validation by checking:
 * 1. Quantity is strictly positive
 * 2. Quantity is an integer
 * 3. Quantity is a multiple of the instrument's lot size
 */
bool Order::validateQuantity(const Instrument& instrument) const
{
    // Verify positive quantity
    if (quantity <= 0)
    {
        std::cout << "ERROR: Quantity must be strictly positive.\n";
        return false;
    }

    // Ensure whole number quantity
    if (std::fmod(quantity, 1.0) != 0.0)
    {
        std::cout << "ERROR: Quantity must be an integer.\n";
        return false;
    }

    // Verify quantity is a multiple of lot size
    if (quantity % instrument.lotsize != 0)
    {
        std::cout << "ERROR: Order quantity must be a multiple of its lot size.\n";
        return false;
    }

    return true;
}
