#include "Client.h"

const std::shared_ptr<Account> Client::getAccount() const {
    return _account;
}

void Client::setAccount(std::shared_ptr<Account> account) {
    _account = account;
}

SOCKET Client::getSc() {
    return _sc;
}

HANDLE Client::getEv() {
    return _ev;
}

const std::string& Client::getIp() {
    return _ip;
}

void Client::setSc(SOCKET sc) {
    _sc = sc;
}

void Client::setEv(HANDLE ev) {
    _ev = ev;
}

void Client::setIp(std::string ip) {
    _ip = ip;
}


