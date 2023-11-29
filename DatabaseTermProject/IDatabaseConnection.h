#ifndef DATABASETERMPROJECT_IDATABASECONNECTION_H
#define DATABASETERMPROJECT_IDATABASECONNECTION_H


#include <string>
using namespace std;

class IDatabaseConnection {
public:
    virtual bool isConnected() = 0;
    virtual bool query(const string& query) = 0;
    virtual void connectionClose() = 0;
private:
    virtual void selectQuery(const string& query) = 0;
    virtual bool commandQuery(const string& query) = 0;
};


#endif //DATABASETERMPROJECT_IDATABASECONNECTION_H
