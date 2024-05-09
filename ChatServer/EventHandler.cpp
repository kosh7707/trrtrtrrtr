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
            ret.emplace_back(std::make_unique<Event>(BROADCAST_INDEX, Event::CHAT_EVENT, contents));
            break;
        }
        case Event::CHAT_CHANNEL_EVENT: {
            // user_id,channel_name,msg
            auto params = split(contents);
            std::string user_id = params[0];
            std::string channel_name = params[1];
            std::string msg = params[2];
            auto sockets = channelManager.channelBroadCast(channel_name);
            for (auto socket : sockets)
                ret.emplace_back(std::make_unique<Event>(observer->socketToIndex[socket], Event::CHAT_EVENT, msg));
            break;
        }
        case Event::CHAT_MAP_EVENT: {
            // user_id,channel_name,map_name,msg
            auto params = split(contents);
            std::string user_id         = params[0];
            std::string channel_name    = params[1];
            std::string map_name        = params[2];
            std::string msg             = params[3];
            auto sockets = channelManager.mapBroadCast(channel_name, map_name);
            for (auto socket : sockets)
                ret.emplace_back(std::make_unique<Event>(observer->socketToIndex[socket], Event::CHAT_EVENT, msg));
            break;
        }
    }

    return ret;
}

