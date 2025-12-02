/**
 * @file InstrumentManager.cpp
 * @brief Implementation of the InstrumentManager class methods
 *
 * Provides functionality for managing and tracking financial instruments
 * in the trading system.
 */

#include "InstrumentManager.hpp"
#include <iostream>

/**
 * @brief Attempts to add a new instrument to the management system
 *
 * @param instrument The instrument to be added
 * @return bool True if the instrument was successfully added, 
 *              false if a duplicate instrument was detected
 *
 * Verifies the uniqueness of the instrument before adding it to:
 * - A set tracking unique instrument identifiers
 * - A vector maintaining the full list of instruments
 * 
 * Provides console feedback about the addition result.
 */
bool InstrumentManager::addInstrument(const Instrument& instrument)
{
    // Check if the instrument is unique before adding
    if (isUniqueInstrument(instrumentSet, instrument))
    {
        // Add instrument to the unique identifier set
        instrumentSet.insert(std::make_tuple(instrument.idinstrument,
                                             instrument.marketIdentificationCode,
                                             instrument.tradingCurrency));

        // Store the full instrument in the instruments vector
        instruments.push_back(instrument);

        // Provide console feedback about successful addition
        std::cout << "Instrument added: " << instrument.idinstrument << "\n";
        return true;
    }
    else
    {
        // Inform user about duplicate instrument
        std::cout << "Instrument already existing\n";
        return false;
    }
}

/**
 * @brief Retrieves the complete list of managed instruments
 *
 * @return const std::vector<Instrument>& Reference to the vector of instruments
 *
 * Provides read-only access to all instruments in the management system.
 * Allows external components to inspect the current set of instruments 
 * without modifying the collection.
 */
const std::vector<Instrument>& InstrumentManager::getInstruments() const
{
    return instruments;
}
