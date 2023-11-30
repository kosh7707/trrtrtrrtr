#ifndef DATABASETERMPROJECT_CLIENT_H
#define DATABASETERMPROJECT_CLIENT_H


#include <winsock2.h>
#include <memory>
#include "Account.h"

class Client {
public:
    const shared_ptr<Account> getAccount() const;
    SOCKET          getSc();
    HANDLE          getEv();
    const string&   getIp();
    void            setAccount(shared_ptr<Account> account);
    void            setSc(SOCKET sc);
    void            setEv(HANDLE ev);
    void            setIp(string ip);
private:
    shared_ptr<Account> _account;
    SOCKET _sc;
    HANDLE _ev;
    string _ip;

};


#endif //DATABASETERMPROJECT_CLIENT_H
