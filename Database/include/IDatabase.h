#ifndef DATABASETERMPROJECT_IDATABASECONNECTION_H
#define DATABASETERMPROJECT_IDATABASECONNECTION_H


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

class IDatabase {
protected:
    using selectQueryResultPtr = std::unique_ptr<std::vector<std::map<std::string, std::string>>>;
public:
    virtual bool   isConnected() = 0;
    virtual bool   commandQuery(const std::string& query) = 0;
    virtual bool   transaction(const std::initializer_list<std::string>& queries) = 0;
    virtual selectQueryResultPtr selectQuery(const std::string& query) = 0;
    virtual ~IDatabase() = default;

    template <typename... Args>
    std::string queryFormatting(Args... formats) {
        char queryBuffer[1024];
        snprintf(queryBuffer, sizeof(queryBuffer), formats...);
        return std::string(queryBuffer);
    }
};


#endif //DATABASETERMPROJECT_IDATABASECONNECTION_H
