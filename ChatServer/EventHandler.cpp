#include "EventHandler.h"

std::vector<std::unique_ptr<Event>> EventHandler::handling(std::unique_ptr<Event> event) {
    int         index       = event->getIndex();
    int         eventCode   = event->getEventCode();
    std::string contents    = event->getContents();

    std::cout << "index: " << index << ", "
              << "eventCode: " << eventCode << ", "
              << "contents: " << contents << std::endl;

    std::vector<std::unique_ptr<Event>> ret;

    switch (eventCode) {
        case Event::CHAT_EVENT: {
            auto params = split(contents);
            std::string user_id         = params[0];
            std::string msg             = params[1];
            auto sockets = channelManager.BroadCast();
            for (auto socket : sockets)
                ret.emplace_back(std::make_unique<Event>(observer->socketToIndex[socket], Event::CHAT_EVENT, user_id + " : " + msg));
            break;
        }
        case Event::CHAT_CHANNEL_EVENT: {
            auto params = split(contents);
            std::string user_id         = params[0];
            std::string channel_name    = params[1];
            std::string msg             = params[2];
            auto sockets = channelManager.channelBroadCast(channel_name);
            for (auto socket : sockets)
                ret.emplace_back(std::make_unique<Event>(observer->socketToIndex[socket], Event::CHAT_EVENT, user_id + " : " + msg));
            break;
        }
        case Event::CHAT_MAP_EVENT: {
            auto params = split(contents);
            std::string user_id         = params[0];
            std::string channel_name    = params[1];
            std::string map_name        = params[2];
            std::string msg             = params[3];
            auto sockets = channelManager.mapBroadCast(channel_name, map_name);
            for (auto socket : sockets)
                ret.emplace_back(std::make_unique<Event>(observer->socketToIndex[socket], Event::CHAT_EVENT, user_id + " : " + msg));
            break;
        }
        case Event::SUB_EVENT: {
            auto params = split(contents);
            std::string user_id         = params[0];
            std::string channel_name    = params[1];
            std::string map_name        = params[2];
            bool res = channelManager.join(observer->indexToSocket[index], channel_name, map_name);
            if (res) {
                auto sockets = channelManager.mapBroadCast(channel_name, map_name);
                for (auto socket : sockets)
                    ret.emplace_back(std::make_unique<Event>(observer->socketToIndex[socket], Event::SUB_SUCCESS_EVENT, user_id));
            }
            else ret.emplace_back(std::make_unique<Event>(index, Event::SUB_FAIL_EVENT, ""));
            break;
        }
        case Event::UNSUB_EVENT: {
            auto params = split(contents);
            std::string user_id         = params[0];
            std::string channel_name    = params[1];
            std::string map_name        = params[2];
            bool res = channelManager.leave(observer->indexToSocket[index], channel_name, map_name);
            if (res) {
                auto sockets = channelManager.mapBroadCast(channel_name, map_name);
                for (auto socket : sockets)
                    ret.emplace_back(std::make_unique<Event>(observer->socketToIndex[socket], Event::UNSUB_SUCCESS_EVENT, user_id));
            }
            else ret.emplace_back(std::make_unique<Event>(index, Event::UNSUB_FAIL_EVENT, ""));
            break;
        }
        default: {
            std::cerr << "Invalid Event" << std::endl;
            ret.emplace_back(std::make_unique<Event>(index, Event::INVALID_EVENT, ""));
            break;
        }
    }

    return ret;
}

