#include "EventHandler.h"

void printUserCommand(bool isLogin);

std::vector<std::unique_ptr<Event>> EventHandler::userInputHandling(const std::string& command) {
    auto ret = std::vector<std::unique_ptr<Event>>();
    try {
        if (!isLogin) {
            std::vector<std::string> cmd = split(command);
            if (cmd[0] == "!help") printUserCommand(isLogin);
            else if (cmd[0] == "!login") ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::LOGIN_EVENT, cmd[1] + "," + cmd[2]));
            else {
                std::cout << "Invalid Command" << std::endl;
            }
        }
        else {
            if (command[0] != '!') ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::CHAT_EVENT, command));
            else {
                std::vector<std::string> cmd = split(command);
                if (cmd[0] == "!help") printUserCommand(isLogin);
                else if (cmd[0] == "!inventoryCheck") ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::INVENTORY_CHECK_EVENT, ""));
                else if (cmd[0] == "!getTestItem") ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::GET_TEST_ITEM_EVENT, cmd[1] + "," + cmd[2]));
                else if (cmd[0] == "!breakItem") ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::BREAK_ITEM_EVENT, cmd[1] + "," + cmd[2]));
                else if (cmd[0] == "!userInfo") ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::USER_INFO_EVENT, ""));
                else {
                    std::cout << "Invalid Command" << std::endl;
                }
            }
        }
        return ret;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return ret;
    }
}

void printUserCommand(bool isLogin) {
    if (!isLogin) {
        std::cout   << "------------------------\n"
                    << "0. !login [id] [pw]\n"
                    << "------------------------"
                    << std::endl;
    }
    else {
        std::cout   << "------------------------\n"
                    << "1. !inventoryCheck\n"
                    << "2. !getTestItem [item_id] [quantity]\n"
                    << "3. !breakItem [item_id] [quantity]\n"
                    << "4. !userInfo\n"
//                    << "3. !sellItem [item_id] [quantity] [buyNowPrice] [startingBidPrice]\n"
//                    << "4. !buyNow [auction_id]\n"
//                    << "5. !bid [auction_id] [price]\n"
//                    << "7. !auctionCheck\n"
                    << "------------------------"
                    << std::endl;
    }
}