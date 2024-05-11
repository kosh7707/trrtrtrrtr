#include "EventHandler.h"

using vectorEventPtr = std::vector<std::unique_ptr<Event>>;

vectorEventPtr EventHandler::handling(std::unique_ptr<Event> event) {
    int         index       = event->getIndex();
    int         eventCode   = event->getEventCode();
    std::string contents    = event->getContents();

    std::cout << "index: " << index << ", "
              << "eventCode: " << eventCode << ", "
              << "contents: " << contents << std::endl;

    vectorEventPtr ret;
    switch (eventCode) {
        case Event::LOGIN_EVENT:
            ret = handleLogin(index, contents);
            break;
        case Event::INVENTORY_CHECK_EVENT:
            ret = handleInventoryCheck(index);
            break;
        case Event::GET_TEST_ITEM_EVENT:
            ret = handleGetTestItem(index, contents);
            break;
        case Event::BREAK_ITEM_EVENT:
            ret = handleBreakItem(index, contents);
            break;
        case Event::USER_INFO_EVENT:
            ret = handleUserInfo(index);
            break;
        case Event::SELL_ITEM_EVENT:
            ret = handleSellItem(index, contents);
            break;
        case Event::OPEN_AUCTION_EVENT:
            ret = handleOpenAuction(index);
            break;
        case Event::BID_EVENT:
            ret = handleBid(index, contents);
            break;
        case Event::BUYNOW_EVENT:
            ret = handleBuyNow(index, contents);
            break;
        default: {
            std::cerr << "Invalid Event" << std::endl;
            ret.emplace_back(std::make_unique<Event>(index, Event::INVALID_EVENT, ""));
            break;
        }
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

vectorEventPtr EventHandler::handleLogin(const int index, const std::string& contents) {
    vectorEventPtr ret;
    auto params = split(contents);
    std::string id = params[0], pw = params[1];
    if (accountService->isExistAccount(id)) {
        auto account = accountService->loginAccount(id, pw);
        if (account == nullptr) ret.emplace_back(std::make_unique<Event>(index, Event::LOGIN_FAIL, ""));
        else {
            ret.emplace_back(std::make_unique<Event>(index, Event::LOGIN_SUCCESS, id));
            auto inventory = inventoryService->getInventory(account->getAccountId());
            {
                std::lock_guard<std::mutex> lock(observer->mutex);
                int socket_id = observer->indexToSocketId[index];
                observer->accountIdToClient[account->getAccountId()] = observer->socketIdToClient[socket_id];
                observer->socketIdToClient[socket_id]->setAccount(std::move(account));
                observer->socketIdToClient[socket_id]->setInventory(std::move(inventory));
            }
        }
    }
    else {
        auto account = accountService->registerAccount(id, pw);
        if (account == nullptr) ret.emplace_back(std::make_unique<Event>(index, Event::REGISTER_FAIL, ""));
        else {
            ret.emplace_back(std::make_unique<Event>(index, Event::LOGIN_SUCCESS, id));
            auto inventory = inventoryService->getInventory(account->getAccountId());
            {
                std::lock_guard<std::mutex> lock(observer->mutex);
                int socket_id = observer->indexToSocketId[index];
                observer->accountIdToClient[account->getAccountId()] = observer->socketIdToClient[socket_id];
                observer->socketIdToClient[socket_id]->setAccount(std::move(account));
                observer->socketIdToClient[socket_id]->setInventory(std::move(inventory));
            }
        }
    }
    return ret;
}

vectorEventPtr EventHandler::handleInventoryCheck(const int index) {
    vectorEventPtr ret;

    int socket_id = observer->indexToSocketId[index];
    auto items = observer->socketIdToClient[socket_id]->getInventory()->getItems();
    std::stringstream ss;
    for (auto item : *items) {
        ss << item.first << ","
           << item.second.second << "|";
    }
    ret.emplace_back(std::make_unique<Event>(index, Event::INVENTORY_CHECK_EVENT, ss.str()));
    return ret;
}

vectorEventPtr EventHandler::handleGetTestItem(const int index, const std::string& contents) {
    vectorEventPtr ret;

    int socket_id = observer->indexToSocketId[index];
    auto params = split(contents);
    int item_id = std::stoi(params[0]); int quantity = std::stoi(params[1]);
    bool res = inventoryService->grantItem(observer->socketIdToClient[socket_id]->getAccount(), observer->socketIdToClient[socket_id]->getInventory(), item_id, quantity);
    if (res) ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, "Get test item, item_id: " + std::to_string(item_id) + ", quantity: " + std::to_string(quantity)));
    else ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, "Fail to get test item."));
    return ret;
}

vectorEventPtr EventHandler::handleBreakItem(const int index, const std::string& contents) {
    vectorEventPtr ret;

    int socket_id = observer->indexToSocketId[index];
    auto params = split(contents);
    int item_id = std::stoi(params[0]); int quantity = std::stoi(params[1]);
    bool res = inventoryService->breakItem(observer->socketIdToClient[socket_id]->getAccount(), observer->socketIdToClient[socket_id]->getInventory(), item_id, quantity);
    if (res) ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, "successfully break the item."));
    else ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, "Fail to break the item."));
    return ret;
}

vectorEventPtr EventHandler::handleUserInfo(const int index) {
    vectorEventPtr ret;

    int socket_id = observer->indexToSocketId[index];
    auto account = observer->socketIdToClient[socket_id]->getAccount().get();
    std::string user_id = account->getUserId();
    int score = account->getScore();
    int balance = account->getBalance();
    std::string contents = user_id + "," + std::to_string(score) + "," + std::to_string(balance);
    ret.emplace_back(std::make_unique<Event>(index, Event::USER_INFO_EVENT, contents));
    return ret;
}

vectorEventPtr EventHandler::handleSellItem(const int index, const std::string& contents) {
    vectorEventPtr ret;

    auto params= split(contents);
    int item_id             = stoi(params[0]);
    int quantity            = stoi(params[1]);
    int buyNowPrice         = stoi(params[2]);
    int startingBidPrice    = stoi(params[3]);

    int socket_id = observer->indexToSocketId[index];
    auto pClient = observer->socketIdToClient[socket_id];
    bool sellItemResult = auctionService->sellItem(pClient->getInventory(), pClient->getAccount()->getAccountId(), item_id, quantity, buyNowPrice, startingBidPrice);
    if (sellItemResult) ret.emplace_back(std::make_unique<Event>(index, Event::SELL_ITEM_SUCCESS_EVENT, ""));
    else ret.emplace_back(std::make_unique<Event>(index, Event::SELL_ITEM_FAIL_EVENT, ""));
    return ret;
}

vectorEventPtr EventHandler::handleOpenAuction(const int index) {
    vectorEventPtr ret;

    std::stringstream ss;
    auto auctions = auctionService->readAuctions();
    for (const auto& auction : *auctions) {
        ss << auction.auction_id << ","
           << auction.item_id << ","
           << auction.item_quantity << ","
           << auction.seller_id << ","
           << auction.start_time << ","
           << auction.end_time << ","
           << auction.current_price << ","
           << auction.current_bidder_id << ","
           << auction.buy_now_price << "|";
    }
    ret.emplace_back(std::make_unique<Event>(index, Event::OPEN_AUCTION_EVENT, ss.str()));
    return ret;
}

vectorEventPtr EventHandler::handleBid(const int index, const std::string& contents) {
    vectorEventPtr ret;

    auto params = split(contents);
    int auction_id  = stoi(params[0]);
    int price       = stoi(params[1]);

    int socket_id = observer->indexToSocketId[index];
    auto bidResult = auctionService->bid(observer->socketIdToClient[socket_id]->getAccount(), auction_id, price);
    if (!bidResult.success) ret.emplace_back(std::make_unique<Event>(index, Event::BID_FAIL_EVENT, ""));
    else {
        ret.emplace_back(std::make_unique<Event>(index, Event::BID_SUCCESS_EVENT, ""));
        if (bidResult.prev_bidder_id != -1) {
            auto prevBidderClient = observer->accountIdToClient[bidResult.prev_bidder_id];
            prevBidderClient->getAccount()->addBalance(bidResult.prev_price);
            ret.emplace_back(std::make_unique<Event>(prevBidderClient->getIndex(), Event::OUTBID_EVENT, ""));
        }
    }
    return ret;
}

vectorEventPtr EventHandler::handleBuyNow(const int index, const std::string& contents) {
    vectorEventPtr ret;

    int auction_id  = stoi(contents);

    int socket_id = observer->indexToSocketId[index];
    auto pClient = observer->socketIdToClient[socket_id];
    auto buyNowResult = auctionService->buyNow(pClient->getAccount(), pClient->getInventory(), auction_id);
    if (!buyNowResult.success) ret.emplace_back(std::make_unique<Event>(index, Event::BUYNOW_FAIL_EVENT, ""));
    else {
        ret.emplace_back(std::make_unique<Event>(index, Event::BUYNOW_SUCCESS_EVENT, ""));
        auto sellerClient = observer->accountIdToClient[buyNowResult.seller_id];
        sellerClient->getAccount()->addBalance(buyNowResult.price);
        ret.emplace_back(std::make_unique<Event>(sellerClient->getIndex(), Event::ITEM_SOLD_EVENT, ""));
        if (buyNowResult.prev_bidder_id != -1) {
            auto prevBidderClient = observer->accountIdToClient[buyNowResult.prev_bidder_id];
            prevBidderClient->getAccount()->addBalance(buyNowResult.prev_price);
            ret.emplace_back(std::make_unique<Event>(prevBidderClient->getIndex(), Event::OUTBID_EVENT, ""));
        }
    }
    return ret;
}