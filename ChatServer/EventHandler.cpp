#include "EventHandler.h"

std::vector<std::unique_ptr<Event>> EventHandler::handling(std::unique_ptr<Event> event) {
    int         index       = event->getIndex();
    int         eventCode   = event->getEventCode();
    std::string contents    = event->getContents();

    std::cout << "index: " << index << ", "
              << "eventCode: " << eventCode << ", "
              << "contents: " << contents << std::endl;

    std::vector<std::unique_ptr<Event>> ret;

    ret.emplace_back(std::make_unique<Event>(index, Event::CHAT_EVENT, contents));

    return ret;
}

