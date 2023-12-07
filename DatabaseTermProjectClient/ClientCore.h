#ifndef DATABASETERMPROJECTCLIENT_CLIENTCORE_H
#define DATABASETERMPROJECTCLIENT_CLIENTCORE_H


#include <iostream>
#include <string>
#include <winsock2.h>
#include <process.h>
#include "ClientConstant.h"
#include "DatabaseConnection.h"

using namespace std;

class ClientCore {
public:
    ClientCore();
    ~ClientCore();
    SOCKET              getSocket();
    void                run();
    void                login();
    bool                getIsLogin();
    void                chat(const string& msg);
    const string&       getUserId() const;
    const string&       getUserPw() const;
    void                connectDB();
    DatabaseConnection& getDatabaseConnection();

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
    DatabaseConnection  dc;
};


#endif //DATABASETERMPROJECTCLIENT_CLIENTCORE_H
