#ifndef CHATSERVER_CONSTANT_H
#define CHATSERVER_CONSTANT_H


#include <string>

class Constant {
public:
    const std::string getConnInfo() {
        return connInfo;
    }
    const std::string getServerIP() {
        return serverIP;
    }
    const int getServerPort() {
        return serverPort;
    }
    const int getMaxClientCount() {
        return maxClientCount;
    }
    const std::string getMainServerIP() const {
        return MainServerIP;
    }
    const int getMainServerPort() const {
        return MainServerPort;
    }
private:
    const std::string connInfo = "dbname=databasetermproject user=kosh7707 password=root hostaddr=127.0.0.1 port=5432";
    const std::string serverIP = "127.0.0.1";
    const int serverPort = 7706;
    const int maxClientCount = 100;

    const std::string   MainServerIP   = "127.0.0.1";
    const int           MainServerPort = 7707;
};


#endif //CHATSERVER_CONSTANT_H
