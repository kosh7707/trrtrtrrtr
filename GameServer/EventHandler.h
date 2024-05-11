#ifndef DATABASETERMPROJECT_EVENTHANDLER_H
#define DATABASETERMPROJECT_EVENTHANDLER_H

#include <sstream>
#include "Service/AccountService.h"
#include "IEventHandler.h"
#include "Observer.h"
#include "Service/InventoryService.h"
#include "Service/AuctionService.h"
#define CHAT_SERVER_INDEX 0

class EventHandler final : public IEventHandler {
    using vectorEventPtr = std::vector<std::unique_ptr<Event>>;
public:
    EventHandler(std::shared_ptr<Observer> observer, std::shared_ptr<AccountService> accountService
                    , std::shared_ptr<InventoryService> inventoryService, std::shared_ptr<AuctionSystem::AuctionService> auctionService)
        : observer(observer), accountService(accountService), inventoryService(inventoryService), auctionService(auctionService) {}

    vectorEventPtr handling(std::unique_ptr<Event> event) override;
    vectorEventPtr userInputHandling(const std::string& command) override {
        return vectorEventPtr();
    }
    ~EventHandler() {}
private:
    std::vector<std::string> split(const std::string& input);
    vectorEventPtr handleLogin(const int index, const std::string& contents);
    vectorEventPtr handleInventoryCheck(const int index);
    vectorEventPtr handleGetTestItem(const int index, const std::string& contents);
    vectorEventPtr handleBreakItem(const int index, const std::string& contents);
    vectorEventPtr handleUserInfo(const int index);
    vectorEventPtr handleSellItem(const int index, const std::string& contents);
    vectorEventPtr handleOpenAuction(const int index);
    vectorEventPtr handleBid(const int index, const std::string& contents);
    vectorEventPtr handleBuyNow(const int index, const std::string& contents);
private:
    std::shared_ptr<AccountService>                 accountService;
    std::shared_ptr<InventoryService>               inventoryService;
    std::shared_ptr<AuctionSystem::AuctionService>  auctionService;
    std::shared_ptr<Observer>                       observer;
};


#endif //DATABASETERMPROJECT_EVENTHANDLER_H
