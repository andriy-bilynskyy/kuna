#include <dbgout.h>
#include <kuna_con.h>


int main()
{
    dbg::dout.setDbgLevel(dbg::DBG_DEBUG); //DBG_INFO DBG_DEBUG
    dbg::dout.setDefaultMsgLevel(dbg::DBG_DEBUG);
    dbg::dout << "KUNA connection test." << dbg::endl;

    kuna_con connector;

    dbg::dout << "server timestamp: " << connector.server_timestamp() << dbg::endl;

    RecentMarketData_t market_data = connector.recent_market_data();
    dbg::dout << "recent market data\n" << market_data.toString() << dbg::endl;

    std::vector<OderBookData_t> order_book = connector.order_book_data();
    dbg::dout << "book order data" << dbg::endl;
    for(std::vector<OderBookData_t>::iterator it = order_book.begin(); it != order_book.end(); ++it)
    {
        dbg::dout << "\n" << it->toString() << dbg::endl;
    }

    std::vector<TradesHistory_t> trades_history = connector.trades_history_data();
    dbg::dout << "book order data" << dbg::endl;
    for(std::vector<TradesHistory_t>::iterator it = trades_history.begin(); it != trades_history.end(); ++it)
    {
        dbg::dout << "\n" << it->toString() << dbg::endl;
    }

    return 0;
}


