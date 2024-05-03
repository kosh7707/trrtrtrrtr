#ifndef SERVERCORE_SOCKET_H
#define SERVERCORE_SOCKET_H


#include <winsock2.h>
#include <string>

class Socket {
public:
    Socket() {}
    Socket(const SOCKET& sc, const HANDLE& ev, const std::string& ip) : sc(sc), ev(ev), ip(ip) {}

    const SOCKET& getSc() const {
        return sc;
    }

    const HANDLE& getEv() const {
        return ev;
    }

    const std::string& getIp() const {
        return ip;
    }

private:
    SOCKET      sc;
    HANDLE      ev;
    std::string ip;
};


#endif //SERVERCORE_SOCKET_H
