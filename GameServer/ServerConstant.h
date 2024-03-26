#ifndef DATABASETERMPROJECT_SERVERCONSTANT_H
#define DATABASETERMPROJECT_SERVERCONSTANT_H


#include <string>
#include <winsock2.h>
#include <thread>
#include <memory>
#include <iostream>
#include <string>
#include <cstring>
#include <process.h>
#include <vector>
#include <queue>

class ServerConstant {
public:
    const std::string getConnInfo() {
        return connInfo;
    }
    const int getServerPort() {
        return serverPort;
    }
    const std::string getServerIP() {
        return serverIP;
    }
    const int getMaxClientCount() {
        return maxClientCount;
    }
private:
    const std::string connInfo = "dbname=databasetermproject user=kosh7707 password=root hostaddr=127.0.0.1 port=5432";
    const std::string serverIP = "127.0.0.1";
    const int serverPort = 7707;
    const int maxClientCount = 100;
};


#endif //DATABASETERMPROJECT_SERVERCONSTANT_H
