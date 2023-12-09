#ifndef DATABASETERMPROJECT_SERVERCONSTANT_H
#define DATABASETERMPROJECT_SERVERCONSTANT_H


#include <string>
using namespace std;

class ServerConstant {
public:
    const string getConnInfo() {
        return connInfo;
    }
    const int getServerPort() {
        return serverPort;
    }
    const string getServerIP() {
        return serverIP;
    }
    const int getMaxClientCount() {
        return maxClientCount;
    }
private:
    const string connInfo = "dbname=databasetermproject user=kosh7707 password=root hostaddr=127.0.0.1 port=5432";
    const string serverIP = "127.0.0.1";
    const int serverPort = 7707;
    const int maxClientCount = 100;
};


#endif //DATABASETERMPROJECT_SERVERCONSTANT_H
