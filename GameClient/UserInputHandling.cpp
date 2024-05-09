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
            if (command[0] != '!') {
                switch (chat_mode) {
                    case 0:
                        ret.emplace_back(std::make_unique<Event>(CHAT_SERVER_INDEX, Event::CHAT_EVENT
                                                                 , user_id + "," + command));
                        break;
                    case 1:
                        ret.emplace_back(std::make_unique<Event>(CHAT_SERVER_INDEX, Event::CHAT_CHANNEL_EVENT
                                                                 , user_id + "," + channel_name + "," + command));
                        break;
                    case 2:
                        ret.emplace_back(std::make_unique<Event>(CHAT_SERVER_INDEX, Event::CHAT_CHANNEL_EVENT
                                                                 , user_id + "," + channel_name + "," + map_name + "," + command));
                        break;
                    default:
                        ret.emplace_back(std::make_unique<Event>(CHAT_SERVER_INDEX, Event::CHAT_EVENT
                                                                 , user_id + "," + command));
                        break;
                }
            }
            else {
                std::vector<std::string> cmd = split(command);
                if (cmd[0] == "!help") printUserCommand(isLogin);
                else if (cmd[0] == "!inventoryCheck") ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::INVENTORY_CHECK_EVENT, ""));
                else if (cmd[0] == "!getTestItem") ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::GET_TEST_ITEM_EVENT, cmd[1] + "," + cmd[2]));
                else if (cmd[0] == "!breakItem") ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::BREAK_ITEM_EVENT, cmd[1] + "," + cmd[2]));
                else if (cmd[0] == "!userInfo") ret.emplace_back(std::make_unique<Event>(MAIN_SERVER_INDEX, Event::USER_INFO_EVENT, ""));
                else if (cmd[0] == "!moveChannel") {
                    ret.emplace_back(std::make_unique<Event>(CHAT_SERVER_INDEX, Event::UNSUB_EVENT, user_id + "," + channel_name + "," + map_name));
                    ret.emplace_back(std::make_unique<Event>(CHAT_SERVER_INDEX, Event::SUB_EVENT, user_id + "," + cmd[1] + "," + map_name));
                }
                else if (cmd[0] == "!moveMap") {
                    ret.emplace_back(std::make_unique<Event>(CHAT_SERVER_INDEX, Event::UNSUB_EVENT, user_id + "," + channel_name + "," + map_name));
                    ret.emplace_back(std::make_unique<Event>(CHAT_SERVER_INDEX, Event::SUB_EVENT, user_id + "," + channel_name + "," + cmd[1]));
                }
                else if (cmd[0] == "!changeChatMode") {
                    int temp = stoi(cmd[1]);
                    if (temp == 0 or temp == 1 or temp == 2) chat_mode = temp;
                    else std::cout << "Invalid chat mode" << std::endl;
                }
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
                    << "5. !moveChannel [channel_name]\n"
                    << "6. !moveMap [map_name]\n"
                    << "7. !changeChatMode 0/1/2\n"
                    << "\t --> 0: to Map\n"
                    << "\t --> 1: to Channel\n"
                    << "\t --> 2: to All\n"
//                    << "3. !sellItem [item_id] [quantity] [buyNowPrice] [startingBidPrice]\n"
//                    << "4. !buyNow [auction_id]\n"
//                    << "5. !bid [auction_id] [price]\n"
//                    << "7. !auctionCheck\n"
                    << "------------------------"
                    << std::endl;
    }
}