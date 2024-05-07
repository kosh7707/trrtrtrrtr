#ifndef DATABASETERMPROJECT_EVENTHANDLER_H
#define DATABASETERMPROJECT_EVENTHANDLER_H

#include <sstream>
#include "Service/AccountService.h"
#include "IEventHandler.h"
#include "Observer.h"

class EventHandler final : public IEventHandler {
public:
    EventHandler(std::shared_ptr<Observer> observer, std::shared_ptr<AccountService> accountService) : observer(observer), accountService(accountService) {}
    std::vector<std::unique_ptr<Event>> handling(std::unique_ptr<Event> event) override;
    std::vector<std::unique_ptr<Event>> userInputHandling(const std::string& command) override {
        return std::vector<std::unique_ptr<Event>>();
    }
    ~EventHandler() {}
private:
    std::vector<std::string> split(const std::string& input);
    std::vector<std::unique_ptr<Event>> handleLogin(const int index, const std::string& contents);
    std::vector<std::unique_ptr<Event>> handleChat(const int index, const std::string& contents);
    std::vector<std::unique_ptr<Event>> handleInventoryCheck(const int index);
private:
    std::shared_ptr<AccountService> accountService;
    std::shared_ptr<Observer> observer;
};


#endif //DATABASETERMPROJECT_EVENTHANDLER_H
