// #include "market_data.hpp"
// #include "utils/logger.hpp"
// #include <algorithm>

// MarketDataManager::MarketDataManager() 
//     : running_(false), thread_pool_(std::make_shared<ThreadPool>(4)) {
// }

// MarketDataManager::~MarketDataManager() {
//     stop();
// }

// bool MarketDataManager::initialize(const Json::Value& config) {
//     try {
//         // Initialize exchange clients
//         if (config["exchanges"]["binance"]["enabled"].asBool()) {
//             binance_client_ = std::make_unique<BinanceClient>();
//             binance_client_->set_orderbook_callback([this](const std::string& data) {
//                 this->process_orderbook_update(data);
//             });
//             binance_client_->set_funding_rate_callback([this](const std::string& symbol, double rate) {
//                 this->process_funding_rate_update(symbol, rate);
//             });
//         }
        
//         if (config["exchanges"]["okx"]["enabled"].asBool()) {
//             okx_client_ = std::make_unique<OKXClient>();
//             okx_client_->set_orderbook_callback([this](const std::string& data) {
//                 this->process_orderbook_update(data);
//             });
//             okx_client_->set_funding_rate_callback([this](const std::string& symbol, double rate) {
//                 this->process_funding_rate_update(symbol, rate);
//             });
//         }
        
//         if (config["exchanges"]["bybit"]["enabled"].asBool()) {
//             bybit_client_ = std::make_unique<BybitClient>();
//             bybit_client_->set_orderbook_callback([this](const std::string& data) {
//                 this->process_orderbook_update(data);
//             });
//         }
        
//         return true;
        
//     } catch (const std::exception& e) {
//         Logger::error("MarketDataManager initialization error: " + std::string(e.what()));
//         return false;
//     }
// }

// void MarketDataManager::start() {
//     if (running_) return;
    
//     running_ = true;
    
//     // Connect to exchanges
//     if (binance_client_) {
//         thread_pool_->enqueue([this]() {
//             if (binance_client_->connect()) {
//                 Logger::info("Binance connected successfully");
//             } else {
//                 Logger::error("Failed to connect to Binance");
//             }
//         });
//     }
    
//     if (okx_client_) {
//         thread_pool_->enqueue([this]() {
//             if (okx_client_->connect()) {
//                 Logger::info("OKX connected successfully");
//             } else {
//                 Logger::error("Failed to connect to OKX");
//             }
//         });
//     }
    
//     if (bybit_client_) {
//         thread_pool_->enqueue([this]() {
//             if (bybit_client_->connect()) {
//                 Logger::info("Bybit connected successfully");
//             } else {
//                 Logger::error("Failed to connect to Bybit");
//             }
//         });
//     }
    
//     // Start monitoring thread
//     monitoring_thread_ = std::thread([this]() {
//         this->monitoring_loop();
//     });
// }

// void MarketDataManager::stop() {
//     if (!running_) return;
    
//     running_ = false;
    
//     if (binance_client_) binance_client_->disconnect();
//     if (okx_client_) okx_client_->disconnect();
//     if (bybit_client_) bybit_client_->disconnect();
    
//     if (monitoring_thread_.joinable()) {
//         monitoring_thread_.join();
//     }
// }

// void MarketDataManager::subscribe_instrument(const Instrument& instrument) {
//     std::lock_guard<std::mutex> lock(instruments_mutex_);
//     instruments_.push_back(instrument);
    
//     // Subscribe to relevant exchanges
//     std::string symbol = instrument.symbol;
    
//     if (instrument.exchange == Exchange::BINANCE && binance_client_) {
//         binance_client_->subscribe_orderbook(symbol);
//         if (instrument.type == InstrumentType::PERPETUAL) {
//             binance_client_->subscribe_funding_rates(symbol);
//         }
//     }
    
//     if (instrument.exchange == Exchange::OKX && okx_client_) {
//         okx_client_->subscribe_orderbook(symbol);
//         if (instrument.type == InstrumentType::PERPETUAL) {
//             okx_client_->subscribe_funding_rates(symbol);
//         }
//     }
    
//     if (instrument.exchange == Exchange::BYBIT && bybit_client_) {
//         bybit_client_->subscribe_orderbook(symbol);
//     }
// }

// void MarketDataManager::process_orderbook_update(const std::string& data) {
//     try {
//         Json::CharReaderBuilder reader;
//         Json::Value root;
//         std::istringstream stream(data);
//         std::string errors;
        
//         if (!Json::parseFromStream(reader, stream, &root, &errors)) {
//             Logger::error("Orderbook JSON parse error: " + errors);
//             return;
//         }
        
//         std::string symbol = root["symbol"].asString();
//         std::string exchange = root["exchange"].asString();
//         std::string key = symbol + "_" + exchange;
        
//         // Create or update orderbook
//         auto orderbook = std::make_shared<OrderBook>();
        
//         // Update bids
//         std::vector<PriceLevel> bids;
//         for (const auto& bid : root["bids"]) {
//             bids.emplace_back(bid["price"].asDouble(), bid["quantity"].asDouble());
//         }
//         orderbook->update_bids(bids);
        
//         // Update asks
//         std::vector<PriceLevel> asks;
//         for (const auto& ask : root["asks"]) {
//             asks.emplace_back(ask["price"].asDouble(), ask["quantity"].asDouble());
//         }
//         orderbook->update_asks(asks);
        
//         // Store orderbook
//         {
//             std::lock_guard<std::mutex> lock(orderbooks_mutex_);
//             orderbooks_[key] = orderbook;
//         }
        
//         // Notify callbacks
//         if (orderbook_callback_) {
//             orderbook_callback_(key, orderbook);
//         }
        
//         updates_processed_++;
        
//     } catch (const std::exception& e) {
//         Logger::error("Orderbook processing error: " + std::string(e.what()));
//     }
// }

// void MarketDataManager::process_funding_rate_update(const std::string& symbol, double rate) {
//     {
//         std::lock_guard<std::mutex> lock(funding_rates_mutex_);
//         funding_rates_[symbol] = rate;
//     }
    
//     if (funding_rate_callback_) {
//         funding_rate_callback_(symbol, rate);
//     }
    
//     Logger::debug("Funding rate update: " + symbol + " = " + std::to_string(rate));
// }

// std::shared_ptr<OrderBook> MarketDataManager::get_orderbook(const std::string& symbol) const {
//     std::lock_guard<std::mutex> lock(orderbooks_mutex_);
//     auto it = orderbooks_.find(symbol);
//     return (it != orderbooks_.end()) ? it->second : nullptr;
// }

// double MarketDataManager::get_funding_rate(const std::string& symbol) const {
//     std::lock_guard<std::mutex> lock(funding_rates_mutex_);
//     auto it = funding_rates_.find(symbol);
//     return (it != funding_rates_.end()) ? it->second : 0.0;
// }

// void MarketDataManager::monitoring_loop() {
//     while (running_) {
//         std::this_thread::sleep_for(std::chrono::seconds(5));
        
//         // Check connection health
//         bool all_connected = true;
        
//         if (binance_client_ && !binance_client_->is_connected()) {
//             all_connected = false;
//             Logger::warning("Binance connection lost");
//         }
        
//         if (okx_client_ && !okx_client_->is_connected()) {
//             all_connected = false;
//             Logger::warning("OKX connection lost");
//         }
        
//         if (bybit_client_ && !bybit_client_->is_connected()) {
//             all_connected = false;
//             Logger::warning("Bybit connection lost");
//         }
        
//         if (!all_connected) {
//             Logger::info("Attempting to reconnect...");
//             // Implement reconnection logic here
//         }
        
//         // Log statistics
//         Logger::info("Updates processed: " + std::to_string(updates_processed_.load()));
//         Logger::info("Active orderbooks: " + std::to_string(orderbooks_.size()));
//     }
// }
