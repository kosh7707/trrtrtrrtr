#ifndef DATABASETERMPROJECT_IDATABASECONNECTION_H
#define DATABASETERMPROJECT_IDATABASECONNECTION_H


#include <string>
#include <libpq-fe.h>
using namespace std;

class IDatabaseConnection {
public:
    virtual bool            isConnected() = 0;
    virtual PGresult*       selectQuery(const string& query) = 0;
    virtual bool            commandQuery(const string& query) = 0;
    virtual void            connectionClose() = 0;
    virtual const PGconn*   getConn() const = 0;
};


#endif //DATABASETERMPROJECT_IDATABASECONNECTION_H
