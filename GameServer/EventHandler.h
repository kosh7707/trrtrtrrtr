#ifndef DATABASETERMPROJECT_EVENTHANDLER_H
#define DATABASETERMPROJECT_EVENTHANDLER_H

#include <sstream>
#include "Service/AccountService.h"
#include "IEventHandler.h"
#include "Observer.h"
#include "Service/InventoryService.h"
#define CHAT_SERVER_INDEX 0

class EventHandler final : public IEventHandler {
public:
    EventHandler(std::shared_ptr<Observer> observer, std::shared_ptr<AccountService> accountService
                    , std::shared_ptr<InventoryService> inventoryService)
        : observer(observer), accountService(accountService), inventoryService(inventoryService) {}

    std::vector<std::unique_ptr<Event>> handling(std::unique_ptr<Event> event) override;
    std::vector<std::unique_ptr<Event>> userInputHandling(const std::string& command) override {
        return std::vector<std::unique_ptr<Event>>();
    }
    ~EventHandler() {}
private:
    std::vector<std::string> split(const std::string& input);
    std::vector<std::unique_ptr<Event>> handleLogin(const int index, const std::string& contents);
    std::vector<std::unique_ptr<Event>> handleInventoryCheck(const int index);
    std::vector<std::unique_ptr<Event>> handleGetTestItem(const int index, const std::string& contents);
    std::vector<std::unique_ptr<Event>> handleBreakItem(const int index, const std::string& contents);
    std::vector<std::unique_ptr<Event>> handleUserInfo(const int index);

private:
    std::shared_ptr<AccountService> accountService;
    std::shared_ptr<InventoryService> inventoryService;
    std::shared_ptr<Observer> observer;
};


#endif //DATABASETERMPROJECT_EVENTHANDLER_H
