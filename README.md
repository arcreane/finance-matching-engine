# EURONEXT Trading Engine 🏢

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![License](https://img.shields.io/badge/license-MIT-blue.svg)]()
[![Version](https://img.shields.io/badge/version-1.0.0-orange.svg)]()

A high-performance trading engine implementation featuring continuous trading with Price-Time Priority (PTP) algorithm.

## Features 🚀

- **Price-Time Priority (PTP) Algorithm**
    - Best price matching
    - Time-based priority for same price level
    - Automatic order matching

- **Order Management**
    - Support for BID/ASK orders
    - Day orders handling
    - GTD (Good Till Date) orders support
    - Real-time order validation

- **Continuous Trading**
    - 24/7 operation capability
    - Thread-safe implementation
    - Real-time order matching
    - Performance monitoring

- **Statistics and Monitoring**
    - Real-time trading statistics
    - Trade history tracking
    - Performance metrics
    - GTD orders monitoring

## Prerequisites 👋

- CMake 3.10 or higher
- C++17 compatible compiler
- Recommended: CLion IDE

## Project Structure 💁

```
EURONEXT-TRADING-ENGINE/
├── MatchingEngine/
│   ├── include/
│   │   ├── Instrument.hpp
│   │   ├── InstrumentManager.hpp
│   │   ├── MatchingEngine.hpp
│   │   ├── Order.hpp
│   │   ├── OrderBook.hpp
│   │   ├── Trading.hpp
│   │   └── Utils.hpp
│   └── src/
│       ├── Instrument.cpp
│       ├── InstrumentManager.cpp
│       ├── Main.cpp
│       ├── MatchingEngine.cpp
│       ├── Order.cpp
│       ├── OrderBook.cpp
│       └── Utils.cpp
└── CMakeLists.txt
```

## Build Instructions 🔨

```bash
# Clone the repository
git clone https://github.com/yourusername/EURONEXT-TRADING-ENGINE.git

# Create build directory
cd EURONEXT-TRADING-ENGINE
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
make
```

## Usage 💻

```bash
# Run the trading engine
./MatchingEngine
```

### Available Commands

| Command  | Description |
|----------|-------------|
| `status` | Display current engine status |
| `stats`  | Display detailed trading statistics |
| `gtd`    | Display GTD orders and their expiry |
| `order`  | Add a new test order |
| `display`| Show order book and trades |
| `help`   | Display help message |
| `quit`   | Stop the engine and exit |

## Key Components 🔑

### Instrument Management
- Unique instrument identification
- Market and currency support
- State management (ACTIVE/INACTIVE/SUSPENDED/DELISTED)

### Order Book
- Price level organization
- Time priority queue
- Efficient order matching

### Matching Engine
- Continuous operation
- Thread-safe order processing
- Real-time statistics tracking

## Performance Metrics 📊

- Real-time matching capabilities
- Nanosecond precision timestamps
- Atomic operations for thread safety
- Efficient memory management

## Authors ✨

- **VILLANO Théo**
- **PAYA Elsa**  
- **CANTRELLE Noa** 
- **LUCET Aymeric**
- **BAYA Sanibé** 

## License 📝

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments 🙏

- Built with modern C++ features
- Inspired by real-world trading systems
- Based on EURONEXT trading principles

---

<div align="center">
Made with ❤️ by EFREI' Students
</div>
