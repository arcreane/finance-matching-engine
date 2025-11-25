/**
* @file Instrument.hpp
* @brief Defines the financial instrument structure and its related components
* 
* This file contains the definition of the Instrument class which represents
* a tradable financial instrument in the trading system, along with its
* associated states and properties.
*/

#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <string>

/**
* @enum State
* @brief Represents the different possible states of a financial instrument
*/
enum class State {
   ACTIVE,     ///< Instrument is available for trading
   INACTIVE,   ///< Instrument is temporarily unavailable
   SUSPENDED,  ///< Trading is suspended for this instrument
   DELISTED    ///< Instrument has been removed from trading
};

/**
* @class Instrument
* @brief Represents a financial instrument in the trading system
*
* Contains all necessary information about a tradable instrument,
* including its identifiers, trading parameters, and current state.
*/
class Instrument {
public:
   int idinstrument;                  ///< Unique identifier (ISIN code)
   std::string marketIdentificationCode; ///< Market where the instrument is traded (MIC)
   std::string tradingCurrency;       ///< Currency used for trading
   char name[50];                     ///< Instrument name (limited to 50 characters)
   int issue;                         ///< Issue number or date
   State state;                       ///< Current trading state
   double refprice;                   ///< Reference price for trading
   int idtradinggroup;               ///< Trading group identifier
   int lotsize;                      ///< Minimum tradable quantity
   int pricedecimal;                 ///< Number of decimal places for price
   int currentorderid;               ///< Current order identifier
   int currenttradeid;               ///< Current trade identifier
   int idapf;                        ///< APF identifier

   /**
    * @brief Constructs a new Instrument object
    *
    * @param idinstrument Unique identifier for the instrument
    * @param marketIdentificationCode Market where the instrument is traded
    * @param tradingCurrency Currency used for trading
    * @param name Instrument name
    * @param issue Issue number or date
    * @param state Current trading state
    * @param refprice Reference price
    * @param idtradinggroup Trading group identifier
    * @param lotsize Minimum tradable quantity
    * @param pricedecimal Number of decimal places for price
    * @param currentorderid Current order identifier
    * @param currenttradeid Current trade identifier
    * @param idapf APF identifier
    */
   Instrument(int idinstrument, const std::string& marketIdentificationCode,
              const std::string& tradingCurrency, const std::string& name,
              int issue, State state, double refprice, int idtradinggroup,
              int lotsize, int pricedecimal, int currentorderid,
              int currenttradeid, int idapf);

   /**
    * @brief Displays the instrument's details
    *
    * Outputs all relevant information about the instrument
    * in a formatted manner to the standard output.
    */
   void display() const;
};

#endif // INSTRUMENT_HPP