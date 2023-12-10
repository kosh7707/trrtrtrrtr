#ifndef DATABASETERMPROJECT_DATABASECONNECTION_H
#define DATABASETERMPROJECT_DATABASECONNECTION_H


#include <iostream>
#include <memory>
#include "ServerConstant.h"
#include "IDatabaseConnection.h"
using namespace std;

class DatabaseConnection : IDatabaseConnection {
public:
    static IDatabaseConnection& getInstance() {
        static DatabaseConnection dc;
        return dc;
    }
    bool isConnected();
    bool commandQuery(const string& query);
    vector<map<string, string>> selectQuery(const string& query);
    bool transaction(const vector<string>& queries);

    template <typename... Args>
    static string queryFormatting(Args... formats) {
        char queryBuffer[1024];
        snprintf(queryBuffer, sizeof(queryBuffer), formats...);
        return string(queryBuffer);
    }
private:
    DatabaseConnection();
    ServerConstant  serverConstant;
    unique_ptr<pqxx::connection> conn;
};


#endif //DATABASETERMPROJECT_DATABASECONNECTION_H
