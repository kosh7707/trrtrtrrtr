#ifndef GAMESERVER_CLIENT_H
#define GAMESERVER_CLIENT_H


#include <memory>
#include "Model/Account.h"

class Client {
public:
    Client() {}
    Client(const int index) : index(index) {}
    int getAccountId();
    std::string getUserId();
    void setAccount(std::unique_ptr<Account> _account);
    void setIndex(const int _index);
private:
    int                      index;
    std::unique_ptr<Account> account;
};


#endif //GAMESERVER_CLIENT_H
