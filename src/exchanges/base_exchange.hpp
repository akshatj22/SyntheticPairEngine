// #pragma once
// #include <string>
// #include <functional>
// #include <memory>
// #include <websocketpp/config/asio_client.hpp>
// #include <websocketpp/client.hpp>
// #include <json/json.hpp>

// typedef websocketpp::client<websocketpp::config::asio_tls_client> ws_client;
// typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

// class BaseExchange {
// protected:
//     ws_client client_;
//     websocketpp::connection_hdl connection_;
//     std::string ws_endpoint_;
//     std::string rest_endpoint_;
//     bool connected_;
    
//     // Callbacks
//     std::function<void(const std::string&)> orderbook_callback_;
//     std::function<void(const std::string&, double)> funding_rate_callback_;
    
//     virtual void on_message(websocketpp::connection_hdl hdl, ws_client::message_ptr msg) = 0;
//     virtual void on_open(websocketpp::connection_hdl hdl);
//     virtual void on_close(websocketpp::connection_hdl hdl);
//     virtual void on_fail(websocketpp::connection_hdl hdl);
    
//     context_ptr on_tls_init(const char* hostname, websocketpp::connection_hdl);

// public:
//     BaseExchange(const std::string& ws_url, const std::string& rest_url);
//     virtual ~BaseExchange();
    
//     virtual bool connect() = 0;
//     virtual void disconnect();
//     virtual void subscribe_orderbook(const std::string& symbol) = 0;
//     virtual void subscribe_funding_rates(const std::string& symbol) = 0;
//     virtual void subscribe_trades(const std::string& symbol) = 0;
    
//     void set_orderbook_callback(std::function<void(const std::string&)> callback);
//     void set_funding_rate_callback(std::function<void(const std::string&, double)> callback);
    
//     bool is_connected() const { return connected_; }
// };
