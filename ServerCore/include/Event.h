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
