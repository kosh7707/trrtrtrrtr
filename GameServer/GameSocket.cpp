#include "GameSocket.h"

const SOCKET& GameSocket::getSc() const {
    return sc;
}

const HANDLE& GameSocket::getEv() const {
    return ev;
}

const std::string& GameSocket::getIp() const {
    return ip;
}

void GameSocket::setSc(const SOCKET& sc) {
    this->sc = sc;
}

void GameSocket::setEv(const HANDLE& ev) {
    this->ev = ev;
}

void GameSocket::setIp(const std::string& ip) {
    this->ip = ip;
}
