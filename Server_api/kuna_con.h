#ifndef KUNA_CON_H_
#define KUNA_CON_H_

#include <string>
#include <vector>
#include <stdint.h>
#include <document.h>

struct RecentMarketData_t
{
    RecentMarketData_t() : timestamp(0), buy(0), sell(0), low(0), high(0),
                           last(0), vol(0), price(0) {}

    std::string toString();

    time_t timestamp;       // server time
    double buy;             // BTC price for buy
    double sell;            // BTC price for sale
    double low;             // the lowest price of the trade in 24 hours
    double high;            // the highest price of the trade in 24 hours
    double last;            // price of the last trade
    double vol;             // volume of trading in base currency for 24 hours
    double price;           // total price of trading in quote currency for 24 hours
};

struct OderBookData_t
{
    OderBookData_t() : id(0), side(), ord_type(), price(0), avg_price(0),
                       state(), market(), created_at(0), volume(0),
                       remaining_volume(0), executed_volume(0), trades_count(0) {}

    std::string toString();

    uint32_t    id;                 // order ID
    std::string side;               // sell or buy
    std::string ord_type;           // limit or market
    double      price;              // price for 1 BTC
    double      avg_price;          // the average trade price for the order
    std::string state;              // order state — always wait
    std::string market;             // market ID
    time_t      created_at;         // the time of placing the order
    double      volume;             // volume of trading in BTC,
    double      remaining_volume;   // unfilled amount of BTC,
    double      executed_volume;    // sold amount of BTC,
    uint32_t    trades_count;       // the number of the trades on the order
};

class kuna_con
{
public:
    kuna_con();
    ~kuna_con();
    std::string server_timestamp();
    RecentMarketData_t recent_market_data();
    std::vector<OderBookData_t> order_book_data();
private:
    std::string request(std::string url);
    static size_t writeCallback(char * data, size_t size, size_t nmemb, std::string * buf);
    bool parse_recent_market_data(const std::string & json, RecentMarketData_t & market);
    bool parse_order_book_data(const std::string & json, std::vector<OderBookData_t> & orderbook);
    bool parse_order(const rapidjson::Value& json, OderBookData_t & order);
};

#endif
