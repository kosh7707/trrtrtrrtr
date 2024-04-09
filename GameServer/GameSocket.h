#ifndef GAMESERVER_GAMESOCKET_H
#define GAMESERVER_GAMESOCKET_H

#include <winsock2.h>
#include <string>

class GameSocket {
public:
    GameSocket() {}

    GameSocket(const SOCKET& sc, const HANDLE& ev, const std::string& ip) : sc(sc), ev(ev), ip(ip) {}

    const SOCKET& getSc() const;

    const HANDLE& getEv() const;

    const std::string& getIp() const;

    void setSc(const SOCKET& sc);

    void setEv(const HANDLE& ev);

    void setIp(const std::string& ip);

private:
    SOCKET      sc;
    HANDLE      ev;
    std::string ip;
};


#endif //GAMESERVER_GAMESOCKET_H
