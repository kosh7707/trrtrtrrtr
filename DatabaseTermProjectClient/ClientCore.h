#ifndef DATABASETERMPROJECTCLIENT_CLIENTCORE_H
#define DATABASETERMPROJECTCLIENT_CLIENTCORE_H


#include <iostream>
#include <string>
#include <winsock2.h>
#include <process.h>
#include <vector>
#include "ClientConstant.h"
#include "DatabaseConnection.h"
#define BUF_SIZE 1024

using namespace std;

class ClientCore {
public:
    ClientCore();
    ~ClientCore();
    void                run();
    void                login();
    bool                getIsLogin();
    void                handleChat(const string& msg);
    void                handleCommand(const vector<string>& command);
    const string&       getUserId() const;
    const string&       getUserPw() const;
    void                connectDB();
    void                notifyServer(const string& msg);

private:
    static unsigned int WINAPI runThread(void* params);
    ClientConstant      clientConstant;
    const string        serverIP;
    const int           serverPort;
    unsigned int        tid;
    SOCKET              sc;
    HANDLE              mainThread;
    bool                isLogin;
    string              userId;
    string              userPw;
};


#endif //DATABASETERMPROJECTCLIENT_CLIENTCORE_H
