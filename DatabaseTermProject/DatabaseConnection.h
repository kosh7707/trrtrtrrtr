#ifndef DATABASETERMPROJECT_DATABASECONNECTION_H
#define DATABASETERMPROJECT_DATABASECONNECTION_H


#include <iostream>
#include <memory>
#include "ServerConstant.h"
#include "IDatabaseConnection.h"

class DatabaseConnection : IDatabaseConnection {
public:
    static IDatabaseConnection& getInstance() {
        static DatabaseConnection dc;
        return dc;
    }
    bool isConnected();
    bool commandQuery(const std::string& query);
    std::vector<std::map<std::string, std::string>> selectQuery(const std::string& query);
    bool transaction(const std::vector<std::string>& queries);

    template <typename... Args>
    static std::string queryFormatting(Args... formats) {
        char queryBuffer[1024];
        snprintf(queryBuffer, sizeof(queryBuffer), formats...);
        return std::string(queryBuffer);
    }
private:
    DatabaseConnection();
    ServerConstant  serverConstant;
    std::unique_ptr<pqxx::connection> conn;
};


#endif //DATABASETERMPROJECT_DATABASECONNECTION_H
