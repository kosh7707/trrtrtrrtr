#ifndef DATABASE_SQLITE_H
#define DATABASE_SQLITE_H


#include "IDatabase.h"

class SQLite : public IDatabase {
public:
    SQLite(const std::string& connInfo) {

    }
    bool isConnected() {

    }
    bool commandQuery(const std::string& query) {

    }
    bool transaction(const std::vector<std::string>& queries) {

    }
    std::vector<std::map<std::string, std::string>> selectQuery(const std::string& query) {

    }
private:

};

#endif //DATABASE_SQLITE_H
