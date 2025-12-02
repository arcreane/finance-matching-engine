/**
 * @file Utils.cpp
 * @brief Implementation of utility functions for instrument management
 *
 * Provides helper functions to verify and manage instrument uniqueness
 * within a trading system.
 */

#include "Utils.hpp"

/**
 * @brief Checks if an instrument is unique in the given set
 *
 * @param instrumentSet Set of existing instruments, tracked by 
 *                      (id, market code, trading currency)
 * @param instrument Instrument to check for uniqueness
 * @return bool True if the instrument is not in the set, false otherwise
 *
 * Determines instrument uniqueness by checking if an identical 
 * combination of instrument ID, market code, and trading currency 
 * already exists in the set.
 */
bool isUniqueInstrument(const std::set<std::tuple<int, std::string, std::string>>& instrumentSet,
                        const Instrument& instrument)
{
    return instrumentSet.find(std::make_tuple(instrument.idinstrument,
                                              instrument.marketIdentificationCode,
                                              instrument.tradingCurrency)) == instrumentSet.end();
}

/**
 * @brief Attempts to add an instrument to the instrument set
 *
 * @param instrumentSet Reference to the set of existing instruments
 * @param instrument Instrument to be added
 * @return bool True if the instrument was successfully added, 
 *              false if a duplicate exists
 *
 * Verifies instrument uniqueness before insertion:
 * - Checks if the instrument already exists in the set
 * - Adds the instrument if it is unique
 * - Returns success status of the addition
 */
bool addInstrument(std::set<std::tuple<int, std::string, std::string>>& instrumentSet,
                   const Instrument& instrument)
{
    if (isUniqueInstrument(instrumentSet, instrument))
    {
        instrumentSet.insert(std::make_tuple(instrument.idinstrument,
                                             instrument.marketIdentificationCode,
                                             instrument.tradingCurrency));
        return true; // Instrument successfully added
    }
    return false; // Instrument already exists
}
