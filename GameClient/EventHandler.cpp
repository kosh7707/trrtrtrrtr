#include "EventHandler.h"

std::vector<std::unique_ptr<Event>> EventHandler::handling(std::unique_ptr<Event> event) {
    int         index       = event->getIndex();
    int         eventCode   = event->getEventCode();
    std::string contents    = event->getContents();

    std::cout << "index: " << index << ", "
              << "eventCode: " << eventCode << ", "
              << "contents: " << contents << std::endl;

    auto ret = std::vector<std::unique_ptr<Event>>();
    switch (eventCode) {
        case Event::LOGIN_SUCCESS:
            std::cout << contents << std::endl;
            isLogin = true;
            break;
        case Event::LOGIN_FAIL:
            std::cout << contents << std::endl;
            isLogin = false;
            break;
        case Event::REGISTER_SUCCESS:
            std::cout << contents << std::endl;
            isLogin = true;
            break;
        case Event::REGISTER_FAIL:
            std::cout << contents << std::endl;
            isLogin = false;
            break;
        case Event::CHAT_EVENT:
            std::cout << contents << std::endl;
            break;
        default:
            std::cerr << "Invalid event" << std::endl;
            break;
    }
    return ret;
}
