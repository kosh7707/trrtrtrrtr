#ifndef DATABASETERMPROJECT_DATABASECONNECTION_H
#define DATABASETERMPROJECT_DATABASECONNECTION_H


#include "IDatabase.h"

class PostgreSQL : IDatabase {
public:
    PostgreSQL(const std::string& connInfo);
    bool isConnected();
    bool commandQuery(const std::string& query);
    std::vector<std::map<std::string, std::string>> selectQuery(const std::string& query);
    bool transaction(const std::vector<std::string>& queries);

private:
    std::unique_ptr<pqxx::connection> conn;
};


#endif //DATABASETERMPROJECT_DATABASECONNECTION_H
