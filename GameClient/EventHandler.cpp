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
            isLogin = true;
            break;
        case Event::LOGIN_FAIL:
            isLogin = false;
            break;
        case Event::REGISTER_SUCCESS:
            isLogin = true;
            break;
        case Event::REGISTER_FAIL:
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

std::vector<std::unique_ptr<Event>> EventHandler::userInputHandling(const std::string& command) {
    auto ret = std::vector<std::unique_ptr<Event>>();
    try {
        std::vector<std::string> cmd = split(command);
        if (cmd[0] == "!help") printUserCommand();
        if (!isLogin) {
            if (cmd[0] == "!login") {
                ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::LOGIN_EVENT, cmd[1] + "," + cmd[2]));
            }
        }
        else {
            // TODO: User 명령어 구현
        }
        return ret;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return ret;
    }
}

void EventHandler::printUserCommand() {
    if (!isLogin) {
        std::cout   << "------------------------\n"
                    << "0. !login [id] [pw]\n"
                    << "------------------------\n";
    }
    else {
        std::cout   << "------------------------\n"
                    << "1. !getTestItem\n"
                    << "2. !inventoryCheck\n"
                    << "3. !sellItem [item_id] [quantity] [buyNowPrice] [startingBidPrice]\n"
                    << "4. !buyNow [auction_id]\n"
                    << "5. !bid [auction_id] [price]\n"
                    << "6. !breakItem [item_id] [quantity]\n"
                    << "7. !auctionCheck\n"
                    << "------------------------\n";
    }
}

std::vector<std::string> EventHandler::split(const std::string& input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;
    while (getline(ss, token, ' '))
        result.push_back(token);
    return result;
}
