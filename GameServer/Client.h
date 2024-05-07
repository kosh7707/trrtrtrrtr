#ifndef GAMESERVER_CLIENT_H
#define GAMESERVER_CLIENT_H


#include <memory>
#include "Model/Account.h"
#include "Model/Inventory.h"

class Client {
public:
    Client() {}
    Client(const int index) : index(index) {}
    int getAccountId();
    std::string getUserId();
    std::unique_ptr<Account>& getAccount();
    std::unique_ptr<Inventory>& getInventory();
    void setAccount(std::unique_ptr<Account> _account);
    void setInventory(std::unique_ptr<Inventory> _inventory);
    void setIndex(const int _index);
private:
    int                         index;
    std::unique_ptr<Account>    account;
    std::unique_ptr<Inventory>  inventory;
};


#endif //GAMESERVER_CLIENT_H
