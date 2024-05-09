#ifndef CHATSERVER_CLIENT_H
#define CHATSERVER_CLIENT_H


#include <string>

class Client {
public:
    Client() {}
    Client(std::string user_id) : user_id(user_id) {}
    void setUserId(std::string _user_id) {
        user_id = _user_id;
    }
    const std::string getUserId() const {
        return user_id;
    }
private:
    std::string user_id;
};


#endif //CHATSERVER_CLIENT_H
