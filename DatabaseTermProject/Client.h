#ifndef DATABASETERMPROJECT_CLIENT_H
#define DATABASETERMPROJECT_CLIENT_H


#include <winsock2.h>
#include <memory>
#include "Account.h"

class Client {
public:
    const std::shared_ptr<Account> getAccount() const;
    SOCKET          getSc();
    HANDLE          getEv();
    const std::string&   getIp();
    void            setAccount(std::shared_ptr<Account> account);
    void            setSc(SOCKET sc);
    void            setEv(HANDLE ev);
    void            setIp(std::string ip);
private:
    std::shared_ptr<Account> _account;
    SOCKET _sc;
    HANDLE _ev;
    std::string _ip;
};


#endif //DATABASETERMPROJECT_CLIENT_H
