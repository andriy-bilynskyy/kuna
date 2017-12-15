#include "kuna_con.h"
#include <curl/curl.h>
#include <dbgout.h>
#include <basic_functions.h>
#include <sstream>


std::string RecentMarketData_t::toString()
{
    std::stringstream result;

    result << "time  : " << ctime(&timestamp);
    result << "buy   : " << buy << '\n';
    result << "sell  : " << sell << '\n';
    result << "low   : " << low << '\n';
    result << "high  : " << high << '\n';
    result << "last  : " << last << '\n';
    result << "vol   : " << vol << '\n';
    result << "price : " << price;

    return result.str();
}

std::string OderBookData_t::toString()
{
    std::stringstream result;

    result << "id               : " << id << '\n';
    result << "side             : " << side << '\n';
    result << "order type       : " << ord_type << '\n';
    result << "price            : " << price << '\n';
    result << "average price    : " << avg_price << '\n';
    result << "state            : " << state << '\n';
    result << "market           : " << market << '\n';
    result << "created at       : " << ctime(&created_at);
    result << "volume           : " << price << '\n';
    result << "remaining volume : " << volume << '\n';
    result << "executed volume  : " << executed_volume << '\n';
    result << "trades count     : " << trades_count;

    return result.str();
}

kuna_con::kuna_con()
{
    dbg::dout  << dbg::info << "kuna connector constructed." << dbg::endl;
}

kuna_con::~kuna_con()
{
    dbg::dout  << dbg::info << "kuna connector destructed." << dbg::endl;
}

std::string kuna_con::server_timestamp()
{
    return request("https://kuna.io/api/v2/timestamp");
}

RecentMarketData_t kuna_con::recent_market_data()
{
    std::string response = request("https://kuna.io/api/v2/tickers/btcuah");
    RecentMarketData_t result;
    if(!parse_recent_market_data(response, result))
    {
        dbg::dout  << dbg::err << "recent market data failed (raw value): '" <<  response << "'" << dbg::endl;
    }
    return result;
}

std::vector<OderBookData_t> kuna_con::order_book_data()
{
    std::string response = request("https://kuna.io/api/v2/order_book?market=btcuah");
    std::vector<OderBookData_t> result;
    if(!parse_order_book_data(response, result))
    {
        dbg::dout  << dbg::err << "order book data failed (raw value): '" <<  response << "'" << dbg::endl;
    }
    return result;
}

std::string kuna_con::request(std::string url)
{
    std::string response = "";
    CURL* curl;

    curl = curl_easy_init();
    if(curl)
    {
        char errorBuffer[CURL_ERROR_SIZE] = {0};
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode result = curl_easy_perform(curl);
        if(result == CURLE_OK)
        {
            dbg::dout  << dbg::info << "raw response: "  << response
                                   << " on request: " << url << dbg::endl;
        }
        else
        {
            dbg::dout  << dbg::err << "curl_easy_perform error: "  << errorBuffer
                                   << " on request: " << url << dbg::endl;
        }
        curl_easy_cleanup(curl);
    }
    else
    {
        dbg::dout  << dbg::err << "curl_easy_init failed." << dbg::endl;
    }
    return response;
}

size_t kuna_con::writeCallback(char * data, size_t size, size_t nmemb, std::string * buf)
{
    size_t result = 0;

    if(buf)
    {
        buf->append(data, size * nmemb);
        result = size * nmemb;
    }
    else
    {
        dbg::dout  << dbg::err << "write callback buffer is NULL." << dbg::endl;
    }
    return result;
}

bool kuna_con::parse_recent_market_data(const std::string & json, RecentMarketData_t & market)
{
    bool result = false;
    rapidjson::Document document;

    rapidjson::ParseResult ok = document.Parse(json.c_str());
    if(ok)
    {
        rapidjson::Value &top = document;
        if(basic::json_get_value(top, "at", market.timestamp) && basic::json_check_object(top, "ticker"))
        {
            rapidjson::Value &intrenal = document["ticker"];
            if(basic::json_get_value(intrenal, "buy", market.buy) &&
               basic::json_get_value(intrenal, "sell", market.sell) &&
               basic::json_get_value(intrenal, "low", market.low) &&
               basic::json_get_value(intrenal, "high", market.high) &&
               basic::json_get_value(intrenal, "last", market.last) &&
               basic::json_get_value(intrenal, "vol", market.vol) &&
               basic::json_get_value(intrenal, "price", market.price))
            {
                result = true;
            }
            else
            {
                dbg::dout  << dbg::err << "wrong json format of market data." << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "wrong json format of market data." << dbg::endl;
        }
    }
    else
    {
        dbg::dout  << dbg::err << "wrong json format." << dbg::endl;
    }
    return result;
}

bool kuna_con::parse_order_book_data(const std::string & json, std::vector<OderBookData_t> & orderbook)
{
    bool result = false;

    rapidjson::Document document;

    rapidjson::ParseResult ok = document.Parse(json.c_str());
    if(ok)
    {
        rapidjson::Value &top = document;
        if(basic::json_check_array(top, "asks") && basic::json_check_array(top, "bids"))
        {
            result = true;
            rapidjson::Value &sell = document["asks"];
            for(rapidjson::SizeType i = 0; i < sell.Size(); i++)
            {
                rapidjson::Value &item = sell[i];
                OderBookData_t order;
                if(parse_order(item, order))
                {
                    orderbook.push_back(order);
                }
                else
                {
                    result = false;
                    dbg::dout  << dbg::err << "wrong json format of book order item." << dbg::endl;
                }
            }

            rapidjson::Value &buy  = document["bids"];
            for(rapidjson::SizeType i = 0; i < buy.Size(); i++)
            {
                rapidjson::Value &item = buy[i];
                OderBookData_t order;
                if(parse_order(item, order))
                {
                    orderbook.push_back(order);
                }
                else
                {
                    result = false;
                    dbg::dout  << dbg::err << "wrong json format of book order item." << dbg::endl;
                }
            }

        }
        else
        {
            dbg::dout  << dbg::err << "wrong json format of book order data." << dbg::endl;
        }
    }
    else
    {
        dbg::dout  << dbg::err << "wrong json format." << dbg::endl;
    }
    return result;
}

bool kuna_con::parse_order(const rapidjson::Value& json, OderBookData_t & order)
{
    bool result = false;
    if(basic::json_get_value(json, "id", order.id)                             &&
       basic::json_get_value(json, "side", order.side)                         &&
       basic::json_get_value(json, "ord_type", order.ord_type)                 &&
       basic::json_get_value(json, "price", order.price)                       &&
       basic::json_get_value(json, "avg_price", order.avg_price)               &&
       basic::json_get_value(json, "state", order.state)                       &&
       basic::json_get_value(json, "market", order.market)                     &&
       basic::json_get_value(json, "created_at", order.created_at)             &&
       basic::json_get_value(json, "volume", order.volume)                     &&
       basic::json_get_value(json, "remaining_volume", order.remaining_volume) &&
       basic::json_get_value(json, "executed_volume", order.executed_volume)   &&
       basic::json_get_value(json, "trades_count", order.trades_count))
    {
        result = true;
    }
    else
    {
        dbg::dout  << dbg::err << "wrong json format of order data." << dbg::endl;
    }
    return result;
}

