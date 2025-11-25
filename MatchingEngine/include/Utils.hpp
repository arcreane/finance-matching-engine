#ifndef UTILS_HPP
#define UTILS_HPP

#include <set>
#include <tuple>
#include <string>
#include "Instrument.hpp"

/**
 * @brief Checks if an instrument is unique within a given set
 *
 * @param instrumentSet Set of existing instruments, stored as tuples of 
 *                      (id, market identification code, trading currency)
 * @param instrument The instrument to verify for uniqueness
 * @return bool True if the instrument is unique, false otherwise
 */
bool isUniqueInstrument(
    const std::set<std::tuple<int, std::string, std::string>>& instrumentSet,
    const Instrument& instrument
);

/**
 * @brief Attempts to add an instrument to the instrument set
 *
 * @param instrumentSet Reference to the set of existing instruments
 * @param instrument The instrument to be added
 * @return bool True if the instrument was successfully added, 
 *              false if it already exists in the set
 */
bool addInstrument(
    std::set<std::tuple<int, std::string, std::string>>& instrumentSet,
    const Instrument& instrument
);

#endif // UTILS_HPP