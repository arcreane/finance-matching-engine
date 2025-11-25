/**
 * @file Instrument.cpp
 * @brief Implementation of the Instrument class methods
 *
 * Provides the core functionality for managing and displaying 
 * financial instrument details in the trading system.
 */

#include "Instrument.hpp"
#include <iostream>
#include <cstring>

/**
 * @brief Constructs a fully initialized Instrument object
 *
 * @param idinstrument Unique identifier for the instrument
 * @param marketIdentificationCode Market Identification Code (MIC)
 * @param tradingCurrency Currency used for trading this instrument
 * @param name Name of the instrument
 * @param issue Instrument issue number
 * @param state Current state of the instrument
 * @param refprice Reference price for the instrument
 * @param idtradinggroup ID of the trading group
 * @param lotsize Minimum trading lot size
 * @param pricedecimal Number of decimal places for price
 * @param currentorderid Current order identifier
 * @param currenttradeid Current trade identifier
 * @param idapf Additional identifier (APF)
 *
 * Initializes all instrument attributes, with special handling 
 * for the name to ensure null-termination and prevent buffer overflows.
 */
Instrument::Instrument(int idinstrument, const std::string& marketIdentificationCode,
                       const std::string& tradingCurrency, const std::string& name,
                       int issue, State state, double refprice, int idtradinggroup,
                       int lotsize, int pricedecimal, int currentorderid, int currenttradeid, int idapf)
    : idinstrument(idinstrument), marketIdentificationCode(marketIdentificationCode), tradingCurrency(tradingCurrency),
      issue(issue), state(state), refprice(refprice), idtradinggroup(idtradinggroup),
      lotsize(lotsize), pricedecimal(pricedecimal), currentorderid(currentorderid), currenttradeid(currenttradeid),
      idapf(idapf)
{
    // Safely copy name with null-termination to prevent buffer overflows
    strncpy(this->name, name.c_str(), sizeof(this->name) - 1);
    this->name[sizeof(this->name) - 1] = '\0'; // Ensure null-termination
}

/**
 * @brief Displays comprehensive details of the instrument
 *
 * Outputs all instrument attributes to the console in a 
 * human-readable, structured format. Provides a complete snapshot 
 * of the instrument's current state and characteristics.
 */
void Instrument::display() const
{
    std::cout << "Instrument ID: " << idinstrument << "\n";
    std::cout << "Market Identification Code (MIC): " << marketIdentificationCode << "\n";
    std::cout << "Trading Currency: " << tradingCurrency << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Issue: " << issue << "\n";

    // Convert state enum to human-readable string
    std::string stateStr;
    switch (state)
    {
    case State::ACTIVE:
        stateStr = "ACTIVE";
        break;
    case State::INACTIVE:
        stateStr = "INACTIVE";
        break;
    case State::SUSPENDED:
        stateStr = "SUSPENDED";
        break;
    case State::DELISTED:
    default:
        stateStr = "DELISTED";
    }
    std::cout << "State: " << stateStr << "\n";

    std::cout << "Reference Price: " << refprice << "\n";
    std::cout << "Trading Group ID: " << idtradinggroup << "\n";
    std::cout << "Lot Size: " << lotsize << "\n";
    std::cout << "Price Decimal: " << pricedecimal << "\n";
    std::cout << "Current Order ID: " << currentorderid << "\n";
    std::cout << "Current Trade ID: " << currenttradeid << "\n";
    std::cout << "APF ID: " << idapf << "\n";
    std::cout << "================================ " << "\n";
}
