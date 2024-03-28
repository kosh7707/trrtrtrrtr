#ifndef DATABASETERMPROJECT_EVENTHANDLER_H
#define DATABASETERMPROJECT_EVENTHANDLER_H

#include "InventoryDAO.h"
#include "AccountDAO.h"
#include "AuctionDAO.h"

class EventHandler {
public:
    std::vector<std::pair<int, std::string>> handling(const int index, const std::string& buf, const int ClientsCount, const std::shared_ptr<Client[]> Clients);
private:
    std::vector<std::string> split(const std::string& input);
    std::string     getMessage(const std::string& buf);
    int             getEvent(const std::string& buf);
    std::vector<std::pair<int, std::string>>    handleLogin(const int index, const std::string& msg, const std::shared_ptr<Client[]> Clients);
    std::vector<std::pair<int, std::string>>    handleChat(const int index, const std::string& msg, const std::shared_ptr<Client[]> Clients);
    std::vector<std::pair<int, std::string>>    handleGetTestItem(const int index, const std::shared_ptr<Client[]> Clients);
    std::vector<std::pair<int, std::string>>    handleInventoryCheck(const int index, const std::shared_ptr<Client[]> Clients);
    std::vector<std::pair<int, std::string>>    handleSellItem(const int index, const std::string& msg, const std::shared_ptr<Client[]> Clients);
    std::vector<std::pair<int, std::string>>    handleBuyNow(const int index, const std::string& msg, const int ClientsCount, const std::shared_ptr<Client[]> Clients);
    std::vector<std::pair<int, std::string>>    handleBid(const int index, const std::string& msg, const int ClientsCount, const std::shared_ptr<Client[]> Clients);
    std::vector<std::pair<int, std::string>>    handleBreakItem(const int index, const std::string& msg, const std::shared_ptr<Client[]> Clients);
    std::vector<std::pair<int, std::string>>    handleAuctionCheck(const int index);
private:
    enum {
        INVALID_EVENT = -1, LOGIN_EVENT, CHAT_EVENT, GET_TEST_ITEM_EVENT, INVENTORY_CHECK_EVENT
        , SELL_ITEM_EVENT, BUY_NOW_EVENT, BID_EVENT, BREAK_ITEM_EVENT, AUCTION_CHECK_EVENT,
    };
    InventoryDAO        inventoryDao;
    AccountDAO          accountDao;
    AuctionDAO          auctionDao;
};


#endif //DATABASETERMPROJECT_EVENTHANDLER_H
