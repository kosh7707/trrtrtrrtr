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
            channel_name = "channel_01";
            map_name = "map_01";
            ret.emplace_back(std::make_unique<Event>(CHAT_SERVER_INDEX, Event::SUB_EVENT, user_id + "," + channel_name + "," + map_name));
            break;
        case Event::LOGIN_FAIL:
            std::cout << "Incorrect username or password." << std::endl;
            isLogin = false;
            break;
        case Event::REGISTER_SUCCESS:
            std::cout << "Successfully registered." << std::endl;
            isLogin = true;
            user_id = contents;
            channel_name = "channel_01";
            map_name = "map_01";
            ret.emplace_back(std::make_unique<Event>(CHAT_SERVER_INDEX, Event::SUB_EVENT, user_id + "," + channel_name + "," + map_name));
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
                std::cout << "item_id: "    << item[0] << "\n"
                          << "item_name: "  << ItemConstant::getItemName(stoi(item[0])) << "\n"
                          << "score: "      << ItemConstant::getItemScore(stoi(item[0])) << "\n"
                          << "quantity: "   << item[1] << "\n"
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
        case Event::SUB_SUCCESS_EVENT: {
            std::cout << contents << " is joined." << std::endl;
            break;
        }
        case Event::UNSUB_SUCCESS_EVENT: {
            std::cout << contents << " is leaved." << std::endl;
            break;
        }
        case Event::SUB_FAIL_EVENT: {
            std::cout << "Fail to enter the channel/map" << std::endl;
            break;
        }
        case Event::UNSUB_FAIL_EVENT: {
            std::cout << "Fail to leave the channel/map" << std::endl;
            break;
        }
        case Event::OPEN_AUCTION_EVENT: {
            auto auctions = split(contents, '|');
            for (auto it : auctions) {
                auto auction = split(it, ',');
                std::cout << "-----------------------\n"
                          << "auction_id: "         << auction[0] << "\n"
                          << "item_id: "            << auction[1] << "\n"
                          << "item_quantity: "      << auction[2] << "\n"
                          << "seller_id: "          << auction[3] << "\n"
                          << "start_time: "         << auction[4] << "\n"
                          << "end_time: "           << auction[5] << "\n"
                          << "current_price: "      << auction[6] << "\n"
                          << "current_bidder_id: "  << auction[7] << "\n"
                          << "buy_now_price: "      << auction[8] << "\n"
                          << "-----------------------"
                          << std::endl;
            }
            break;
        }
        case Event::SELL_ITEM_SUCCESS_EVENT: {
            std::cout << "Item has been successfully registered in the auction." << std::endl;
            break;
        }
        case Event::SELL_ITEM_FAIL_EVENT: {
            std::cout << "Failed to register the item in the auction." << std::endl;
            break;
        }
        case Event::BID_SUCCESS_EVENT: {
            std::cout << "Bid has been successfully placed in the auction." << std::endl;
            break;
        }
        case Event::BID_FAIL_EVENT: {
            std::cout << "Failed to place the bid in the auction." << std::endl;
            break;
        }
        case Event::BUYNOW_SUCCESS_EVENT: {
            std::cout << "Item has been successfully bought in the auction." << std::endl;
            break;
        }
        case Event::BUYNOW_FAIL_EVENT: {
            std::cout << "Failed to buy the item in the auction." << std::endl;
            break;
        }
        case Event::OUTBID_EVENT: {
            std::cout << "Your auction has been outbid. please check your inventory." << std::endl;
            break;
        }
        case Event::ITEM_SOLD_EVENT: {
            std::cout << "Your Item has benn sold, please check your inventory." << std::endl;
            break;
        }
        default:
            std::cerr << "Invalid event" << std::endl;
            break;
    }
    return ret;
}
