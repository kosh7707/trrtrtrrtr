#ifndef DATABASETERMPROJECT_DATABASECONNECTION_H
#define DATABASETERMPROJECT_DATABASECONNECTION_H


#include <iostream>
#include <string>
#include <libpq-fe.h>
#include "ServerConstant.h"
#include "IDatabaseConnection.h"
using namespace std;

class DatabaseConnection : IDatabaseConnection{
public:
    static IDatabaseConnection& getInstance() {
        static DatabaseConnection dc;
        return dc;
    }
    bool isConnected();
    bool query(const string& query);
    void connectionClose();
private:
    const char* stoca(const string& str);
    void selectQuery(const string& query);
    bool commandQuery(const string& query);
    DatabaseConnection();
    ServerConstant serverConstant;
    PGconn* conn;
};


#endif //DATABASETERMPROJECT_DATABASECONNECTION_H
