#ifndef SERVERCORE_SOCKET_H
#define SERVERCORE_SOCKET_H


#include <winsock2.h>
#include <string>

class Socket {
public:
    static inline int uuid = 0;

    Socket() { socket_id = uuid++; }
    Socket(const SOCKET& sc, const HANDLE& ev, const std::string& ip) : sc(sc), ev(ev), ip(ip) { socket_id = uuid++; }

    const int getSocketId() const {
        return socket_id;
    }

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
    int         socket_id;
    SOCKET      sc;
    HANDLE      ev;
    std::string ip;
};


#endif //SERVERCORE_SOCKET_H
