#ifndef DATABASETERMPROJECT_IDATABASECONNECTION_H
#define DATABASETERMPROJECT_IDATABASECONNECTION_H


#include <string>
#include <vector>
#include <map>
#include <pqxx/pqxx>
using namespace std;

class IDatabaseConnection {
public:
    virtual bool isConnected() = 0;
    virtual bool commandQuery(const string& query) = 0;
    virtual vector<map<string, string>> selectQuery(const string& query) = 0;
    virtual bool transaction(const vector<string>& queries) = 0;
};


#endif //DATABASETERMPROJECT_IDATABASECONNECTION_H
