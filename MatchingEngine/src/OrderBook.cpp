/**
 * @file OrderBook.cpp
 * @brief Implementation of the OrderBook class methods
 *
 * Provides core functionality for managing trading orders,
 * executing order matching, and tracking trade history.
 */

#include "OrderBook.hpp"
#include <iomanip>
#include "MatchingEngine.hpp"
#include <algorithm>

/**
 * @brief Default constructor for OrderBook
 *
 * Initializes the order book with default values:
 * - Sets initial trade ID to 1
 * - Sets matching engine pointer to null
 */
OrderBook::OrderBook() : nextTradeId(1), matchingEngine(nullptr)
{
}

/**
 * @brief Notifies the matching engine about a new trade
 *
 * @param trade The completed trade to be reported
 *
 * Updates trading statistics in the matching engine if
 * a matching engine is available.
 */
void OrderBook::notifyMatch(const Trade& trade)
{
    if (matchingEngine)
    {
        matchingEngine->updateStats(trade);
    }
}

/**
 * @brief Adds a new order to the appropriate order container
 *
 * @param order The order to be added
 *
 * Inserts the order into either BID or ASK orders map
 * based on its order type.
 */
void OrderBook::addOrder(const Order& order)
{
    // Insert BID orders into bidOrders map
    if (order.ordertype == OrderType::BID)
    {
        bidOrders[order.price].push_back(order);
    }
    // Insert ASK orders into askOrders map
    else if (order.ordertype == OrderType::ASK)
    {
        askOrders[order.price].push_back(order);
    }
}

/**
 * @brief Attempts to match BID and ASK orders
 *
 * @return int Number of trades executed
 *
 * Implements the core order matching algorithm:
 * - Matches orders across price levels
 * - Executes trades when matching conditions are met
 * - Handles partial order fulfillment
 * - Cleans up fully executed orders
 */
int OrderBook::matchOrders()
{
    int tradesExecuted = 0;
    std::lock_guard<std::mutex> lock(displayMutex);

    while (!bidOrders.empty() && !askOrders.empty())
    {
        auto highestBidIt = bidOrders.begin();
        auto lowestAskIt = askOrders.begin();

        // Stop matching if lowest ask price exceeds highest bid price
        if (highestBidIt->first < lowestAskIt->first)
        {
            break;
        }

        bool matchFound = false;
        for (auto& bidOrder : highestBidIt->second)
        {
            for (auto& askOrder : lowestAskIt->second)
            {
                // Verify order compatibility (instrument, market, currency)
                if (bidOrder.idinstrument == askOrder.idinstrument &&
                    bidOrder.marketIdentificationCode == askOrder.marketIdentificationCode &&
                    bidOrder.tradingCurrency == askOrder.tradingCurrency)
                {
                    auto now = std::chrono::system_clock::now();
                    auto now_time_t = std::chrono::system_clock::to_time_t(now);

                    // Log matching order details
                    std::cout << "\nMatching orders found at "
                        << std::put_time(std::localtime(&now_time_t), "%H:%M:%S") << ":\n"
                        << "BID: " << bidOrder.idorder << " Price: "
                        << std::fixed << std::setprecision(2) << bidOrder.price << "\n"
                        << "ASK: " << askOrder.idorder << " Price: "
                        << askOrder.price << std::endl;

                    // Determine trade quantity (minimum of bid and ask quantities)
                    int tradeQuantity = std::min(bidOrder.quantity, askOrder.quantity);

                    // Create trade record
                    Trade trade;
                    trade.tradeId = nextTradeId++;
                    trade.buyOrderId = bidOrder.idorder;
                    trade.sellOrderId = askOrder.idorder;
                    trade.marketIdentificationCode = bidOrder.marketIdentificationCode;
                    trade.tradingCurrency = bidOrder.tradingCurrency;
                    trade.price = askOrder.price;
                    trade.quantity = tradeQuantity;
                    trade.timestamp = now;

                    // Record and notify about the trade
                    trades.push_back(trade);
                    notifyMatch(trade);

                    // Update remaining order quantities
                    bidOrder.quantity -= tradeQuantity;
                    askOrder.quantity -= tradeQuantity;

                    tradesExecuted++;
                    matchFound = true;

                    // Log trade execution details
                    std::cout << "Executed trade: " << tradeQuantity
                        << " units at price " << trade.price << std::endl;

                    // Stop processing if either order is fully executed
                    if (bidOrder.quantity == 0 || askOrder.quantity == 0)
                    {
                        break;
                    }
                }
            }
            if (matchFound) break;
        }

        // Remove fully executed orders
        cleanupExecutedOrders();
        if (!matchFound) break;
    }

    return tradesExecuted;
}

/**
 * @brief Removes orders with zero remaining quantity
 *
 * Cleans up both BID and ASK order containers by:
 * - Removing orders with zero quantity
 * - Removing empty price levels
 */
void OrderBook::cleanupExecutedOrders()
{
    // Clean up BID orders
    for (auto it = bidOrders.begin(); it != bidOrders.end();)
    {
        it->second.erase(
            std::remove_if(it->second.begin(), it->second.end(),
                           [](const Order& o) { return o.quantity == 0; }),
            it->second.end()
        );
        if (it->second.empty())
        {
            it = bidOrders.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Clean up ASK orders
    for (auto it = askOrders.begin(); it != askOrders.end();)
    {
        it->second.erase(
            std::remove_if(it->second.begin(), it->second.end(),
                           [](const Order& o) { return o.quantity == 0; }),
            it->second.end()
        );
        if (it->second.empty())
        {
            it = askOrders.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

/**
 * @brief Displays the current state of the order book
 *
 * Outputs all BID and ASK orders sorted by price levels,
 * providing a comprehensive view of current market orders.
 */
void OrderBook::displayOrderBook() const
{
    std::cout << "\n\n============== ORDER BOOK ==============\n\n";

    std::cout << "\nBID Orders=====================\n";
    for (const auto& bid : bidOrders)
    {
        std::cout << "Price LEVEL: " << std::fixed << std::setprecision(2) << bid.first << "\n";
        for (const auto& order : bid.second)
        {
            order.display();
        }
    }

    std::cout << "\n\nASK Orders=====================\n";
    for (const auto& ask : askOrders)
    {
        std::cout << "Price LEVEL: " << std::fixed << std::setprecision(2) << ask.first << "\n";
        for (const auto& order : ask.second)
        {
            order.display();
        }
    }

    std::cout << "\n\n============== END OF ORDER BOOK ==============\n";
}

/**
 * @brief Displays the history of executed trades
 *
 * Outputs details of all trades that have been completed,
 * or a message if no trades have occurred.
 */
void OrderBook::displayTrades() const
{
    std::cout << "\n\n============== TRADE HISTORY ==============\n";

    if (trades.empty())
    {
        std::cout << "No trades have been executed yet.\n";
    }
    else
    {
        for (const auto& trade : trades)
        {
            trade.display();
        }
    }

    std::cout << "\n\n============== END OF TRADE HISTORY ==============\n";
}

/**
 * @brief Displays comprehensive details of a trade
 *
 * Outputs all relevant information about a single trade
 * execution, including identifiers, pricing, and timing.
 */
void Trade::display() const
{
    std::time_t trade_time = std::chrono::system_clock::to_time_t(timestamp);
    std::cout << "====== Trade Information =======\n";
    std::cout << "Trade ID: " << tradeId << "\n";
    std::cout << "Buy Order ID: " << buyOrderId << "\n";
    std::cout << "Sell Order ID: " << sellOrderId << "\n";
    std::cout << "Market Identification Code (MIC): " << marketIdentificationCode << "\n";
    std::cout << "Trading Currency: " << tradingCurrency << "\n";
    std::cout << "Price: " << std::fixed << std::setprecision(2) << price << "\n";
    std::cout << "Quantity: " << quantity << "\n";
    std::cout << "Timestamp: " << std::ctime(&trade_time);
    std::cout << "================================\n";
}

/**
 * @brief Retrieves the most recently executed trade
 *
 * @return const Trade* Pointer to the last trade, or nullptr if no trades exist
 */
const Trade* OrderBook::getLastTrade() const
{
    return trades.empty() ? nullptr : &trades.back();
}
