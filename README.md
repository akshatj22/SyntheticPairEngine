# Synthetic Pair Engine

**Author:** Akshat Jain

## Overview

Synthetic Pair Engine is a modular C++17/20 backend system for detecting arbitrage opportunities between real and synthetic crypto instruments (spot, futures, perpetuals, options). The engine is designed for extensibility, real-time data processing, and advanced risk management.

## Features

- Real-time or simulated market data processing
- Synthetic and real instrument price comparison
- Cross-exchange and multi-instrument arbitrage detection
- Modular architecture: easy to extend for new strategies or exchanges
- Risk management: VaR, liquidity, and position limits
- Clean, maintainable codebase with clear separation of concerns

## Project Structure
![image](https://github.com/user-attachments/assets/dd62c31a-d2d9-47a6-a0bc-6d6aaf290424)

## Quick Start

### Prerequisites

- C++17 or later
- CMake 3.16+
- [nlohmann/json](https://github.com/nlohmann/json)
- [websocketpp](https://github.com/zaphoyd/websocketpp) (for live exchange integration)
- [Boost](https://www.boost.org/) (for threading/networking)

### Build
`git clone https://github.com/yourusername/SYNTHETICPAIRENGINE.git`

`cd SYNTHETICPAIRENGINE`

`mkdir build && cd build`

`cmake ..`

`cmake --build .`

### Run

`./SyntheticPairEngine.exe`


You will see simulated market data, arbitrage detection, and risk metrics output in the terminal.

## Main Components

- **arbitrage_engine.hpp/cpp**: Core arbitrage detection logic and opportunity management.
- **mispricing_detector.hpp/cpp**: Identifies mispricings and arbitrage signals.
- **pricing_models.hpp/cpp**: Synthetic pricing models for spot, futures, perpetuals, and options.
- **exchange_websocket.hpp**: Interfaces for live market data feeds (ready for extension).
- **exposure_management.hpp**: Position sizing and risk control.
- **main.cpp**: Demo runner and entry point.

## Configuration

- Edit parameters (profit thresholds, risk limits, etc.) in `arbitrage_engine.hpp` or as needed.
- For live data, extend `exchange_websocket.hpp` and implement exchange-specific logic in `src/exchanges/`.

## Documentation

- Each header and source file contains inline documentation for classes and methods.
- The codebase is organized for clarity and ease of extension.
- For more details, see comments in the respective files.

## License

MIT License

---

*“Synthetic Pair Engine: Modular, fast, and extensible arbitrage detection for crypto markets.”*
This README is concise, covers your folder structure , and provides clear build/run instructions, a project overview, and a component summary. Adjust the GitHub URL as needed.

Related
What key sections should I include in my README.md for clarity
How can I effectively highlight main features and usage instructions
Should I add badges to improve the visual appeal of my README
How do I structure the project overview to attract contributors
Can you suggest a concise template for documenting installation steps
