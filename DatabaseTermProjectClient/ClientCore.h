#ifndef DATABASETERMPROJECTCLIENT_CLIENTCORE_H
#define DATABASETERMPROJECTCLIENT_CLIENTCORE_H


#include <iostream>
#include <string>
#include <winsock2.h>
#include <process.h>
#include <vector>
#include "ClientConstant.h"
#define BUF_SIZE 1024

enum {
    INVALID_EVENT = -1, LOGIN_EVENT, CHAT_EVENT
};

using namespace std;

class ClientCore {
public:
    ClientCore();
    ~ClientCore();
    void                run();
    void                login(const string& id, const string& pw);
    void                sendChat(const string& msg);
    void                sendCommand(const vector<string>& command);
    bool                getIsLogin();

private:
    ClientConstant      clientConstant;
    const string        serverIP;
    const int           serverPort;
    unsigned int        tid;
    SOCKET              sc;
    HANDLE              mainThread;
    bool                isLogin;
    string              userId;
    string              userPw;

    const string&       getUserId() const;
    const string&       getUserPw() const;
    void                notifyServer(const string& msg);
    void                ReadServer();
    void                handleLogin(const string& msg);
    void                handleChat(const string& msg);
    string              getMessage(const char *buf);
    int                 getEvent(const char *buf);
    static unsigned int WINAPI runThread(void* params);

};


#endif //DATABASETERMPROJECTCLIENT_CLIENTCORE_H
