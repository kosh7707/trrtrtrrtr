#ifndef DATABASETERMPROJECT_EVENTHANDLER_H
#define DATABASETERMPROJECT_EVENTHANDLER_H

#include "InventoryDAO.h"
#include "AccountDAO.h"
#include "AuctionDAO.h"

class ServerCore;

class EventHandler {
public:
    EventHandler(ServerCore* serverCore);
    void handling(const int index, const char* buf);
private:
    std::vector<std::string> split(const std::string &input);
    int     getEvent(const char* buf);
    std::string  getMessage(const char* buf);
    void    notifyClient(const int index, const std::string& msg, const int event);
    void    notifyAllClients(const std::string& msg);
    void    handleLogin(const int index, const std::string& msg);
    void    handleChat(const int index, const std::string& msg);
    void    handleGetTestItem(const int index);
    void    handleInventoryCheck(const int index);
    void    handleSellItem(const int index, const std::string& msg);
    void    handleBuyNow(const int index, const std::string& msg);
    void    handleBid(const int index, const std::string& msg);
    void    handleBreakItem(const int index, const std::string& msg);
    void    handleAuction(const int index);
private:
    enum {
        INVALID_EVENT = -1, LOGIN_EVENT, CHAT_EVENT, GET_TEST_ITEM_EVENT, INVENTORY_CHECK_EVENT
        , SELL_ITEM_EVENT, BUY_NOW_EVENT, BID_EVENT, BREAK_ITEM_EVENT, AUCTION_CHECK_EVENT,
    };
    ServerCore*         serverCore;
    InventoryDAO        inventoryDao;
    AccountDAO          accountDao;
    AuctionDAO          auctionDao;
};


#endif //DATABASETERMPROJECT_EVENTHANDLER_H
