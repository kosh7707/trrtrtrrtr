#include "DatabaseConnection.h"

DatabaseConnection::DatabaseConnection() {
    string connInfo = serverConstant.getConnInfo();
    conn = make_unique<pqxx::connection>(connInfo);
}

bool DatabaseConnection::isConnected() {
    return conn->is_open();
}

vector<map<string, string>> DatabaseConnection::selectQuery(const string& query) {
    try {
        pqxx::work txn(*conn);
        pqxx::result res = txn.exec(query);

        vector<map<string, string>> ret;
        for (const auto& row : res) {
            map<string, string> temp_map;
            for (const auto& field : row)
                temp_map[field.name()] = field.c_str();
            ret.push_back(temp_map);
        }
        return ret;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return vector<map<string, string>>();
    }
}

bool DatabaseConnection::commandQuery(const string& query) {
    try {
        pqxx::work txn(*conn);
        txn.exec(query);
        txn.commit();
        return true;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return false;
    }
}

bool DatabaseConnection::transaction(const vector<string>& queries) {
    try {
        pqxx::work txn(*conn);
        for (const auto& query : queries)
            txn.exec(query);
        txn.commit();
        return true;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return false;
    }
}


