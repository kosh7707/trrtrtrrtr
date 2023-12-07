#ifndef DATABASETERMPROJECT_DATABASECONNECTION_H
#define DATABASETERMPROJECT_DATABASECONNECTION_H


#include <iostream>
#include <string>
#include <libpq-fe.h>
#include "ClientConstant.h"
using namespace std;

class DatabaseConnection{
public:
    DatabaseConnection() {}
    void connectDB(const string& id, const string& pw);
    bool        isConnected();
    PGresult*   selectQuery(const string& query);
    bool        commandQuery(const string& query);
    void        connectionClose();
    PGconn*     getConn();
private:
    ClientConstant  clientConstant;
    PGconn*         conn;
};


#endif //DATABASETERMPROJECT_DATABASECONNECTION_H
