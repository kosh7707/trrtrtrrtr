#ifndef DATABASETERMPROJECT_DATABASECONNECTION_H
#define DATABASETERMPROJECT_DATABASECONNECTION_H


#include <iostream>
#include <string>
#include <libpq-fe.h>
#include "ClientConstant.h"
#include "IDatabaseConnection.h"
using namespace std;

class DatabaseConnection : IDatabaseConnection {
public:
    static IDatabaseConnection& getInstance() {
        static DatabaseConnection dc;
        return dc;
    }
    bool        isConnected();
    PGresult*   selectQuery(const string& query);
    bool        commandQuery(const string& query);
    void        connectionClose();
private:
    DatabaseConnection();
    ClientConstant  clientConstant;
    PGconn*         conn;
};


#endif //DATABASETERMPROJECT_DATABASECONNECTION_H
