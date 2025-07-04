// #include "models/orderbook.hpp"
// #include <algorithm>

// void OrderBook::update_bids(const std::vector<PriceLevel>& new_bids) {
//     std::unique_lock<std::shared_mutex> lock(mutex_);
    
//     bids = new_bids;
    
//     // Sort bids in descending order (highest price first)
//     std::sort(bids.begin(), bids.end(), 
//               [](const PriceLevel& a, const PriceLevel& b) {
//                   return a.price > b.price;
//               });
    
//     last_update = std::chrono::system_clock::now();
// }

// void OrderBook::update_asks(const std::vector<PriceLevel>& new_asks) {
//     std::unique_lock<std::shared_mutex> lock(mutex_);
    
//     asks = new_asks;
    
//     // Sort asks in ascending order (lowest price first)
//     std::sort(asks.begin(), asks.end(), 
//               [](const PriceLevel& a, const PriceLevel& b) {
//                   return a.price < b.price;
//               });
    
//     last_update = std::chrono::system_clock::now();
// }

// double OrderBook::get_best_bid() const {
//     std::shared_lock<std::shared_mutex> lock(mutex_);
//     return bids.empty() ? 0.0 : bids[0].price;
// }

// double OrderBook::get_best_ask() const {
//     std::shared_lock<std::shared_mutex> lock(mutex_);
//     return asks.empty() ? 0.0 : asks[0].price;
// }

// double OrderBook::get_mid_price() const {
//     std::shared_lock<std::shared_mutex> lock(mutex_);
    
//     if (bids.empty() || asks.empty()) {
//         return 0.0;
//     }
    
//     return (bids[0].price + asks[0].price) / 2.0;
// }

// double OrderBook::get_spread() const {
//     std::shared_lock<std::shared_mutex> lock(mutex_);
    
//     if (bids.empty() || asks.empty()) {
//         return 0.0;
//     }
    
//     return asks[0].price - bids[0].price;
// }

// std::vector<PriceLevel> OrderBook::get_bids(size_t depth) const {
//     std::shared_lock<std::shared_mutex> lock(mutex_);
    
//     size_t max_depth = std::min(depth, bids.size());
//     return std::vector<PriceLevel>(bids.begin(), bids.begin() + max_depth);
// }

// std::vector<PriceLevel> OrderBook::get_asks(size_t depth) const {
//     std::shared_lock<std::shared_mutex> lock(mutex_);
    
//     size_t max_depth = std::min(depth, asks.size());
//     return std::vector<PriceLevel>(asks.begin(), asks.begin() + max_depth);
// }

// std::chrono::system_clock::time_point OrderBook::get_last_update() const {
//     std::shared_lock<std::shared_mutex> lock(mutex_);
//     return last_update;
// }
