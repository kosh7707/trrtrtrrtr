#ifndef CHATSERVER_EVENTHANDLER_H
#define CHATSERVER_EVENTHANDLER_H


#include <iostream>
#include <sstream>
#include "IEventHandler.h"
#include "ChannelManager.h"
#include "Observer.h"

class EventHandler final : public IEventHandler {
public:
    EventHandler(std::shared_ptr<Observer> observer) : observer(observer) {}
    ~EventHandler() {}
    std::vector<std::unique_ptr<Event>> handling(std::unique_ptr<Event> event) override;
    std::vector<std::unique_ptr<Event>> userInputHandling(const std::string &command) override {
        return std::vector<std::unique_ptr<Event>>();
    }
private:
    std::vector<std::string> split(const std::string& input) {
        std::vector<std::string> result;
        std::stringstream ss(input);
        std::string token;
        while (getline(ss, token, ','))
            result.push_back(token);
        return result;
    }
    std::shared_ptr<Observer> observer;
    std::vector<std::string> users;
    ChannelManager channelManager;
};


#endif //CHATSERVER_EVENTHANDLER_H
