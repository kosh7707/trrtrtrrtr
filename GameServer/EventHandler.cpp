#include "EventHandler.h"

std::vector<std::unique_ptr<Event>> EventHandler::handling(std::unique_ptr<Event> event) {
    int         index       = event->getIndex();
    int         eventCode   = event->getEventCode();
    std::string contents    = event->getContents();

    std::cout << "index: " << index << ", "
              << "eventCode: " << eventCode << ", "
              << "contents: " << contents << std::endl;

    std::vector<std::unique_ptr<Event>> ret;
    if (eventCode == Event::LOGIN_EVENT) {
        ret = handleLogin(index, contents);
    }
    else if (eventCode == Event::CHAT_EVENT) {
        ret = handleChat(index, contents);
    }

    return ret;
}

std::vector<std::string> EventHandler::split(const std::string& input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;
    while (getline(ss, token, ','))
        result.push_back(token);
    return result;
}

std::vector<std::unique_ptr<Event>> EventHandler::handleLogin(const int index, const std::string& contents) {
    std::vector<std::unique_ptr<Event>> ret;
    auto params = split(contents);
    std::string id = params[0], pw = params[1];
    if (accountService->isExistAccount(id)) {
        auto account = accountService->loginAccount(id, pw);
        if (account == nullptr) ret.emplace_back(std::make_unique<Event>(index, Event::LOGIN_FAIL, "Incorrect username or password."));
        else {
            ret.emplace_back(std::make_unique<Event>(index, Event::LOGIN_SUCCESS, "Successfully logged in. Welcome " + id + "."));
            SOCKET sc = observer->indexToSocket[index].getSc();
            observer->socketToClient[sc].setAccount(std::move(account));
        }
    }
    else {
        auto account = accountService->registerAccount(id, pw);
        if (account == nullptr) ret.emplace_back(std::make_unique<Event>(index, Event::REGISTER_FAIL, "Registration failed."));
        else {
            ret.emplace_back(std::make_unique<Event>(index, Event::LOGIN_SUCCESS, "Successfully registered. Welcome " + id + "."));
            SOCKET sc = observer->indexToSocket[index].getSc();
            observer->socketToClient[sc].setAccount(std::move(account));
        }
    }
    return ret;
}

std::vector<std::unique_ptr<Event>> EventHandler::handleChat(const int index, const std::string& contents) {
    std::vector<std::unique_ptr<Event>> ret;

    SOCKET sc = observer->indexToSocket[index].getSc();
    std::string user_id = observer->socketToClient[sc].getUserId();
    ret.emplace_back(std::make_unique<Event>(BROADCAST_INDEX, Event::CHAT_EVENT, user_id + " : " + contents));
    return ret;
}

std::vector<std::unique_ptr<Event>> EventHandler::userInputHandling(const std::string& command) {
    return std::vector<std::unique_ptr<Event>>();
}

//std::vector<std::pair<int, std::string>> EventHandler::handleGetTestItem(const int index, const std::unique_ptr<Client[]>& Clients) {
//    std::vector<std::pair<int, std::string>> ret;
//    int account_id = Clients[index].getAccount()->getAccountId();
//    bool res = inventoryDao.insertItem(account_id, 1);
//    if (res) {
//        ret.push_back({index, "[1]Give you test item."});
//        std::cout << "grant user '" + std::to_string(account_id) + "' test item." << std::endl;
//    }
//    else{
//        ret.push_back({index, "[1]error , can't give you test item."});
//        std::cout << "error, can't grant user '" + std::to_string(account_id) + "' test item." << std::endl;
//    }
//    return ret;
//}

//std::vector<std::pair<int, std::string>> EventHandler::handleInventoryCheck(const int index, const std::unique_ptr<Client[]>& Clients) {
//    std::vector<std::pair<int, std::string>> ret;
//    int account_id = Clients[index].getAccount()->getAccountId();
//    std::string res = inventoryDao.inventoryCheck(account_id);
//    ret.push_back({index, "[1]" + res});
//    return ret;
//}
//
//std::vector<std::pair<int, std::string>> EventHandler::handleSellItem(const int index, const std::string& msg, const std::unique_ptr<Client[]>& Clients) {
//    std::vector<std::pair<int, std::string>> ret;
//    int account_id = Clients[index].getAccount()->getAccountId();
//    std::vector<std::string> params = split(msg);
//    bool res = inventoryDao.sellItem(account_id, stoi(params[0]), stoi(params[1]), stoi(params[2]), stoi(params[3]));
//    if (res) ret.push_back({index, "[1]Item has been successfully registered in the auction."});
//    else ret.push_back({index, "[1]Failed to register the item in the auction."});
//    return ret;
//}
//
//std::vector<std::pair<int, std::string>> EventHandler::handleBuyNow(const int index, const std::string& msg, const int ClientsCount, const std::unique_ptr<Client[]>& Clients) {
//    std::vector<std::pair<int, std::string>> ret;
//    int account_id = Clients[index].getAccount()->getAccountId();
//    std::vector<std::string> params = split(msg);
//    std::pair<bool, std::pair<int, int>> res = inventoryDao.buyNow(account_id, stoi(params[0]));
//    if (res.first) {
//        ret.push_back({index, "[1]Item has been successfully purchased in the auction."});
//        for (int i=1; i<ClientsCount; i++) {
//            if (Clients[i].getAccount()->getAccountId() == res.second.first)
//                ret.push_back({i, "[1]your item has been sold to " + Clients[i].getAccount()->getUserId()});
//            else if (Clients[i].getAccount()->getAccountId() == res.second.second)
//                ret.push_back({i, "[1]Your auction has been outbid. please check your inventory"});
//        }
//    }
//    else ret.push_back({index, "[1]Failed to purchase the item in the auction."});
//    return ret;
//}
//
//std::vector<std::pair<int, std::string>> EventHandler::handleBid(const int index, const std::string& msg, const int ClientsCount, const std::unique_ptr<Client[]>& Clients) {
//    std::vector<std::pair<int, std::string>> ret;
//    int account_id = Clients[index].getAccount()->getAccountId();
//    std::vector<std::string> params = split(msg);
//    std::pair<bool, int> res = inventoryDao.bid(account_id, stoi(params[0]), stoi(params[1]));
//    if (res.first) {
//        ret.push_back({index, "[1]Bid has been successfully placed in the auction."});
//        for (int i=1; i<ClientsCount; i++) {
//            if (Clients[i].getAccount()->getAccountId() == res.second)
//                ret.push_back({i, "[1]Your auction has been outbid. please check your inventory"});
//        }
//    }
//    else ret.push_back({index, "[1]Failed to place the bid in the auction."});
//    return ret;
//}
//
//std::vector<std::pair<int, std::string>> EventHandler::handleBreakItem(const int index, const std::string& msg, const std::unique_ptr<Client[]>& Clients) {
//    std::vector<std::pair<int, std::string>> ret;
//    int account_id = Clients[index].getAccount()->getAccountId();
//    std::vector<std::string> params = split(msg);
//    bool res = inventoryDao.breakItem(account_id, stoi(params[0]), stoi(params[1]));
//    if (res) ret.push_back({index, "[1]Item has been successfully breaked in the inventory."});
//    else ret.push_back({index, "[1]Failed to break the item."});
//    return ret;
//}
//
//std::vector<std::pair<int, std::string>> EventHandler::handleAuctionCheck(const int index) {
//    std::vector<std::pair<int, std::string>> ret;
//    std::string res = auctionDao.auctionCheck();
//    ret.push_back({index, "[1]" + res});
//    return ret;
//}


