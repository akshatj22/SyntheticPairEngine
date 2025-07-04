// #include "binance_client.hpp"
// #include "utils/logger.hpp"
// #include <sstream>

// BinanceClient::BinanceClient() 
//     : BaseExchange("wss://stream.binance.com:9443/ws", "https://api.binance.com/api/v3") {
// }

// bool BinanceClient::connect() {
//     try {
//         client_.clear_access_channels(websocketpp::log::alevel::all);
//         client_.clear_error_channels(websocketpp::log::elevel::all);
//         client_.init_asio();
        
//         client_.set_message_handler([this](websocketpp::connection_hdl hdl, ws_client::message_ptr msg) {
//             this->on_message(hdl, msg);
//         });
        
//         client_.set_open_handler([this](websocketpp::connection_hdl hdl) {
//             this->on_open(hdl);
//         });
        
//         client_.set_close_handler([this](websocketpp::connection_hdl hdl) {
//             this->on_close(hdl);
//         });
        
//         client_.set_fail_handler([this](websocketpp::connection_hdl hdl) {
//             this->on_fail(hdl);
//         });
        
//         client_.set_tls_init_handler([this](websocketpp::connection_hdl hdl) {
//             return this->on_tls_init("stream.binance.com", hdl);
//         });
        
//         websocketpp::lib::error_code ec;
//         ws_client::connection_ptr con = client_.get_connection(ws_endpoint_, ec);
        
//         if (ec) {
//             Logger::error("Binance connection error: " + ec.message());
//             return false;
//         }
        
//         connection_ = con->get_handle();
//         client_.connect(con);
        
//         // Start the client in a separate thread
//         std::thread([this]() {
//             client_.run();
//         }).detach();
        
//         return true;
        
//     } catch (const std::exception& e) {
//         Logger::error("Binance connect exception: " + std::string(e.what()));
//         return false;
//     }
// }

// void BinanceClient::subscribe_orderbook(const std::string& symbol) {
//     if (!connected_) {
//         Logger::error("Binance not connected, cannot subscribe to orderbook");
//         return;
//     }
    
//     Json::Value subscribe_msg;
//     subscribe_msg["method"] = "SUBSCRIBE";
//     subscribe_msg["params"] = Json::arrayValue;
//     subscribe_msg["params"].append(symbol + "@depth20@100ms");
//     subscribe_msg["id"] = 1;
    
//     Json::StreamWriterBuilder builder;
//     std::string message = Json::writeString(builder, subscribe_msg);
    
//     websocketpp::lib::error_code ec;
//     client_.send(connection_, message, websocketpp::frame::opcode::text, ec);
    
//     if (ec) {
//         Logger::error("Binance orderbook subscription error: " + ec.message());
//     } else {
//         Logger::info("Subscribed to Binance orderbook: " + symbol);
//     }
// }

// void BinanceClient::subscribe_funding_rates(const std::string& symbol) {
//     // Binance funding rates are available via REST API
//     // For real-time updates, we'd need to poll periodically
//     Logger::info("Binance funding rates subscription for: " + symbol);
// }

// void BinanceClient::subscribe_trades(const std::string& symbol) {
//     if (!connected_) {
//         Logger::error("Binance not connected, cannot subscribe to trades");
//         return;
//     }
    
//     Json::Value subscribe_msg;
//     subscribe_msg["method"] = "SUBSCRIBE";
//     subscribe_msg["params"] = Json::arrayValue;
//     subscribe_msg["params"].append(symbol + "@trade");
//     subscribe_msg["id"] = 2;
    
//     Json::StreamWriterBuilder builder;
//     std::string message = Json::writeString(builder, subscribe_msg);
    
//     websocketpp::lib::error_code ec;
//     client_.send(connection_, message, websocketpp::frame::opcode::text, ec);
    
//     if (ec) {
//         Logger::error("Binance trades subscription error: " + ec.message());
//     }
// }

// void BinanceClient::on_message(websocketpp::connection_hdl hdl, ws_client::message_ptr msg) {
//     try {
//         std::string payload = msg->get_payload();
        
//         Json::CharReaderBuilder reader;
//         Json::Value root;
//         std::istringstream stream(payload);
//         std::string errors;
        
//         if (!Json::parseFromStream(reader, stream, &root, &errors)) {
//             Logger::error("Binance JSON parse error: " + errors);
//             return;
//         }
        
//         // Handle depth updates
//         if (root.isMember("stream") && root["stream"].asString().find("@depth") != std::string::npos) {
//             process_orderbook_update(root["data"]);
//         }
//         // Handle trade updates
//         else if (root.isMember("stream") && root["stream"].asString().find("@trade") != std::string::npos) {
//             process_trade_update(root["data"]);
//         }
        
//     } catch (const std::exception& e) {
//         Logger::error("Binance message processing error: " + std::string(e.what()));
//     }
// }

// void BinanceClient::process_orderbook_update(const Json::Value& data) {
//     if (!orderbook_callback_) return;
    
//     try {
//         std::string symbol = data["s"].asString();
        
//         // Create standardized orderbook format
//         Json::Value orderbook;
//         orderbook["symbol"] = symbol;
//         orderbook["exchange"] = "binance";
//         orderbook["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(
//             std::chrono::system_clock::now().time_since_epoch()).count();
        
//         // Process bids
//         orderbook["bids"] = Json::arrayValue;
//         for (const auto& bid : data["bids"]) {
//             Json::Value level;
//             level["price"] = std::stod(bid[0].asString());
//             level["quantity"] = std::stod(bid[1].asString());
//             orderbook["bids"].append(level);
//         }
        
//         // Process asks
//         orderbook["asks"] = Json::arrayValue;
//         for (const auto& ask : data["asks"]) {
//             Json::Value level;
//             level["price"] = std::stod(ask[0].asString());
//             level["quantity"] = std::stod(ask[1].asString());
//             orderbook["asks"].append(level);
//         }
        
//         Json::StreamWriterBuilder builder;
//         std::string orderbook_str = Json::writeString(builder, orderbook);
        
//         orderbook_callback_(orderbook_str);
        
//     } catch (const std::exception& e) {
//         Logger::error("Binance orderbook processing error: " + std::string(e.what()));
//     }
// }

// void BinanceClient::process_trade_update(const Json::Value& data) {
//     // Process trade data for market analysis
//     Logger::debug("Binance trade update: " + data["s"].asString() + 
//                   " Price: " + data["p"].asString() + 
//                   " Quantity: " + data["q"].asString());
// }
