#ifndef SERVERCORE_EVENT_H
#define SERVERCORE_EVENT_H


#include <string>

class Event {
public:
    enum {
        INVALID_EVENT = -1,
        LOGIN_EVENT = 1,
        LOGIN_FAIL = 2,
        LOGIN_SUCCESS = 3,
        REGISTER_FAIL = 4,
        REGISTER_SUCCESS = 5,
        CHAT_EVENT = 6,
        INVENTORY_CHECK_EVENT = 7,
        GET_TEST_ITEM_EVENT = 8,
        BREAK_ITEM_EVENT = 9,
        USER_INFO_EVENT = 10,
        CHAT_CHANNEL_EVENT = 11,
        CHAT_MAP_EVENT = 12,
        SUB_EVENT = 13,
        UNSUB_EVENT = 14,
        SUB_SUCCESS_EVENT = 15,
        SUB_FAIL_EVENT = 16,
        UNSUB_SUCCESS_EVENT = 17,
        UNSUB_FAIL_EVENT = 18,
    };
    Event(int index, int eventCode, const std::string& contents) : index(index), eventCode(eventCode), contents(contents) {}

    const int getIndex() const {
        return index;
    }

    const int getEventCode() const {
        return eventCode;
    }

    const std::string& getContents() const {
        return contents;
    }

private:
    const int index;
    const int eventCode;
    const std::string contents;
};


#endif //SERVERCORE_EVENT_H
