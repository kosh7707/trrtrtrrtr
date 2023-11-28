#ifndef DATABASETERMPROJECT_SERVERCONSTANT_H
#define DATABASETERMPROJECT_SERVERCONSTANT_H


#include <string>
using namespace std;

class ServerConstant {
public:
    ServerConstant() { }
    const string getDBName() {
        return dbName;
    }
    const string getDBId() {
        return dbId;
    }
    const string getdbPw() {
        return dbPw;
    }
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
    const string dbName = "databasetermproject";
    const string dbId = "kosh7707";
    const string dbPw = "root";
    const string connInfo = "postgresql://postgres@localhost?port=5432&dbname=" + dbName + "&user=" + dbId + "&password=" + dbPw;
    const int serverPort = 7707;
    const string serverIP = "127.0.0.1";
    const int maxClientCount = 100;
};


#endif //DATABASETERMPROJECT_SERVERCONSTANT_H
