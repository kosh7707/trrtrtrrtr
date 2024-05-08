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
            std::cout << "Successfully logged in." << std::endl;
            isLogin = true;
            user_id = contents;
            break;
        case Event::LOGIN_FAIL:
            std::cout << "Incorrect username or password." << std::endl;
            isLogin = false;
            break;
        case Event::REGISTER_SUCCESS:
            std::cout << "Successfully registered." << std::endl;
            isLogin = true;
            user_id = contents;
            break;
        case Event::REGISTER_FAIL:
            std::cout << "Registration failed." << std::endl;
            isLogin = false;
            break;
        case Event::CHAT_EVENT:
            std::cout << contents << std::endl;
            break;
        case Event::INVENTORY_CHECK_EVENT: {
            auto items = split(contents, '|');
            std::cout << "-----------------------\n";
            for (auto it : items) {
                auto item = split(it, ',');
                std::cout << "item_id: "  << item[0] << "\n"
                          << "score: "    << item[1] << "\n"
                          << "quantity: " << item[2] << "\n"
                          << "-----------------------"
                          << std::endl;
            }
            break;
        }
        case Event::USER_INFO_EVENT: {
            auto user = split(contents, ',');
            std::cout << "-----------------------\n"
                      << "user_id: " << user[0] << "\n"
                      << "score: "   << user[1] << "\n"
                      << "balance: " << user[2] << "\n"
                      << "-----------------------"
                      << std::endl;
            break;
        }
        default:
            std::cerr << "Invalid event" << std::endl;
            break;
    }
    return ret;
}
