#ifndef DATABASETERMPROJECT_IDATABASECONNECTION_H
#define DATABASETERMPROJECT_IDATABASECONNECTION_H


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "pqxx/pqxx"

class IDatabase {
public:
    virtual bool   isConnected() = 0;
    virtual bool   commandQuery(const std::string& query) = 0;
    virtual bool   transaction(const std::vector<std::string>& queries) = 0;
    virtual std::vector<std::map<std::string, std::string>> selectQuery(const std::string& query) = 0;

    template <typename... Args>
    std::string queryFormatting(Args... formats) {
        char queryBuffer[1024];
        snprintf(queryBuffer, sizeof(queryBuffer), formats...);
        return std::string(queryBuffer);
    }
};


#endif //DATABASETERMPROJECT_IDATABASECONNECTION_H
