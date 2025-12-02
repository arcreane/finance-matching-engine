/**
* @file InstrumentManager.hpp
* @brief Manages the collection and validation of financial instruments
*
* This class handles the storage and management of financial instruments,
* ensuring uniqueness and providing access to the instrument collection.
*/

#ifndef INSTRUMENT_MANAGER_HPP
#define INSTRUMENT_MANAGER_HPP

#include <set>
#include <vector>
#include "Instrument.hpp"
#include "Utils.hpp"

/**
* @class InstrumentManager
* @brief Manages a collection of financial instruments
*
* Provides functionality to add and retrieve instruments while
* maintaining uniqueness constraints based on instrument identifiers.
*/
class InstrumentManager {
private:
    /// Stores unique instrument identifiers as tuples of (id, market code, currency)
    std::set<std::tuple<int, std::string, std::string>> instrumentSet;

    /// Collection of all valid instruments
    std::vector<Instrument> instruments;

public:
    /**
     * @brief Adds a new instrument to the collection
     *
     * Verifies the uniqueness of the instrument based on its identifier tuple
     * (id, market code, currency) before adding it to the collection.
     *
     * @param instrument The instrument to be added
     * @return true if the instrument was successfully added
     * @return false if the instrument already exists
     */
    bool addInstrument(const Instrument& instrument);

    /**
     * @brief Retrieves the collection of all valid instruments
     *
     * @return const std::vector<Instrument>& Reference to the vector of instruments
     */
    const std::vector<Instrument>& getInstruments() const;
};

#endif // INSTRUMENT_MANAGER_HPP