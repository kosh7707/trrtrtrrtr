#ifndef DATABASETERMPROJECT_DATABASECONNECTION_H
#define DATABASETERMPROJECT_DATABASECONNECTION_H


#include <iostream>
#include <memory>
#include "IDatabaseConnection.h"
using namespace std;

class DatabaseConnection : IDatabaseConnection {
public:
    static IDatabaseConnection& getInstance() {
        static DatabaseConnection dc;
        return dc;
    }
    bool isConnected();
    void connectDB(const string& id, const string& pw);
    bool commandQuery(const string& query);
    vector<map<string, string>> selectQuery(const string& query);
    bool transaction(const vector<string>& queries);
private:
    DatabaseConnection();
    unique_ptr<pqxx::connection> conn;
};


#endif //DATABASETERMPROJECT_DATABASECONNECTION_H
