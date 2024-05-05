#ifndef DATABASETERMPROJECT_DATABASECONNECTION_H
#define DATABASETERMPROJECT_DATABASECONNECTION_H

#include <string>
#include <vector>
#include <map>
#include <pqxx/pqxx>
#include "IDatabase.h"

class PostgreSQL : public IDatabase {
public:
    PostgreSQL(const std::string& connInfo) {
        conn = std::make_unique<pqxx::connection>(connInfo);
    }
    bool isConnected() {
        return conn->is_open();
    }
    bool commandQuery(const std::string& query) {
        try {
            pqxx::work txn(*conn);
            txn.exec(query);
            txn.commit();
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }
    std::vector<std::map<std::string, std::string>> selectQuery(const std::string& query) {
        try {
            pqxx::work txn(*conn);
            pqxx::result res = txn.exec(query);

            std::vector<std::map<std::string, std::string>> ret;
            for (const auto& row : res) {
                std::map<std::string, std::string> temp_map;
                for (const auto& field : row)
                    temp_map[field.name()] = field.c_str();
                ret.push_back(temp_map);
            }
            return ret;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return std::vector<std::map<std::string, std::string>>();
        }
    }
    bool transaction(const std::vector<std::string>& queries) {
        try {
            pqxx::work txn(*conn);
            for (const auto& query : queries)
                txn.exec(query);
            txn.commit();
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

private:
    std::unique_ptr<pqxx::connection> conn;
};


#endif //DATABASETERMPROJECT_DATABASECONNECTION_H
