// #include "okx_client.hpp"
// #include "utils/logger.hpp"

// OKXClient::OKXClient() 
//     : BaseExchange("wss://ws.okx.com:8443/ws/v5/public", "https://www.okx.com/api/v5") {
// }

// bool OKXClient::connect() {
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
        
//         websocketpp::lib::error_code ec;
//         ws_client::connection_ptr con = client_.get_connection(ws_endpoint_, ec);
        
//         if (ec) {
//             Logger::error("OKX connection error: " + ec.message());
//             return false;
//         }
        
//         connection_ = con->get_handle();
//         client_.connect(con);
        
//         std::thread([this]() {
//             client_.run();
//         }).detach();
        
//         return true;
        
//     } catch (const std::exception& e) {
//         Logger::error("OKX connect exception: " + std::string(e.what()));
//         return false;
//     }
// }

// void OKXClient::subscribe_orderbook(const std::string& symbol) {
//     if (!connected_) return;
    
//     Json::Value subscribe_msg;
//     subscribe_msg["op"] = "subscribe";
//     subscribe_msg["args"] = Json::arrayValue;
    
//     Json::Value arg;
//     arg["channel"] = "books";
//     arg["instId"] = symbol;
//     subscribe_msg["args"].append(arg);
    
//     Json::StreamWriterBuilder builder;
//     std::string message = Json::writeString(builder, subscribe_msg);
    
//     websocketpp::lib::error_code ec;
//     client_.send(connection_, message, websocketpp::frame::opcode::text, ec);
    
//     if (!ec) {
//         Logger::info("Subscribed to OKX orderbook: " + symbol);
//     }
// }

// void OKXClient::subscribe_funding_rates(const std::string& symbol) {
//     if (!connected_) return;
    
//     Json::Value subscribe_msg;
//     subscribe_msg["op"] = "subscribe";
//     subscribe_msg["args"] = Json::arrayValue;
    
//     Json::Value arg;
//     arg["channel"] = "funding-rate";
//     arg["instId"] = symbol;
//     subscribe_msg["args"].append(arg);
    
//     Json::StreamWriterBuilder builder;
//     std::string message = Json::writeString(builder, subscribe_msg);
    
//     websocketpp::lib::error_code ec;
//     client_.send(connection_, message, websocketpp::frame::opcode::text, ec);
// }

// void OKXClient::on_message(websocketpp::connection_hdl hdl, ws_client::message_ptr msg) {
//     try {
//         std::string payload = msg->get_payload();
        
//         Json::CharReaderBuilder reader;
//         Json::Value root;
//         std::istringstream stream(payload);
//         std::string errors;
        
//         if (!Json::parseFromStream(reader, stream, &root, &errors)) {
//             Logger::error("OKX JSON parse error: " + errors);
//             return;
//         }
        
//         if (root.isMember("data")) {
//             for (const auto& data : root["data"]) {
//                 if (root["arg"]["channel"].asString() == "books") {
//                     process_orderbook_update(data);
//                 } else if (root["arg"]["channel"].asString() == "funding-rate") {
//                     process_funding_rate_update(data);
//                 }
//             }
//         }
        
//     } catch (const std::exception& e) {
//         Logger::error("OKX message processing error: " + std::string(e.what()));
//     }
// }

// void OKXClient::process_orderbook_update(const Json::Value& data) {
//     if (!orderbook_callback_) return;
    
//     try {
//         Json::Value orderbook;
//         orderbook["symbol"] = data["instId"].asString();
//         orderbook["exchange"] = "okx";
//         orderbook["timestamp"] = std::stoll(data["ts"].asString());
        
//         // Process bids and asks
//         orderbook["bids"] = Json::arrayValue;
//         for (const auto& bid : data["bids"]) {
//             Json::Value level;
//             level["price"] = std::stod(bid[0].asString());
//             level["quantity"] = std::stod(bid[1].asString());
//             orderbook["bids"].append(level);
//         }
        
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
//         Logger::error("OKX orderbook processing error: " + std::string(e.what()));
//     }
// }

// void OKXClient::process_funding_rate_update(const Json::Value& data) {
//     if (!funding_rate_callback_) return;
    
//     try {
//         std::string symbol = data["instId"].asString();
//         double funding_rate = std::stod(data["fundingRate"].asString());
        
//         funding_rate_callback_(symbol, funding_rate);
        
//     } catch (const std::exception& e) {
//         Logger::error("OKX funding rate processing error: " + std::string(e.what()));
//     }
// }
