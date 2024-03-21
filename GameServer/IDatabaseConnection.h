#ifndef DATABASETERMPROJECT_IDATABASECONNECTION_H
#define DATABASETERMPROJECT_IDATABASECONNECTION_H


#include <string>
#include <vector>
#include <map>
#include <pqxx/pqxx>

class IDatabaseConnection {
public:
    virtual bool isConnected() = 0;
    virtual bool commandQuery(const std::string& query) = 0;
    virtual std::vector<std::map<std::string, std::string>> selectQuery(const std::string& query) = 0;
    virtual bool transaction(const std::vector<std::string>& queries) = 0;
};


#endif //DATABASETERMPROJECT_IDATABASECONNECTION_H
