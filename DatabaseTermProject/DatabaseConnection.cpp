#include "DatabaseConnection.h"

bool DatabaseConnection::isConnected() {
    return PQstatus(conn) == CONNECTION_OK;
}

bool DatabaseConnection::query(const string& query) {
    bool ret = true;
    string temp = query.substr(0, 6);
    for (auto& it : temp) it = tolower(it);
    if (temp == "select") selectQuery(query);
    else ret = commandQuery(query);
    return ret;
}

void DatabaseConnection::connectionClose() {
    PQfinish(conn);
}

const char* DatabaseConnection::stoca(const string& str) {
    return str.c_str();
}

DatabaseConnection::DatabaseConnection() {
    const string connInfo = serverConstant.getConnInfo();
    conn = PQconnectdb(stoca(connInfo));
    if (PQstatus(conn) != CONNECTION_OK)
        PQfinish(conn);
}

void DatabaseConnection::selectQuery(const std::string& query) {
    PGresult* res = PQexec(conn, stoca(query));
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cout << "select query: " << query << "\nselect failed." << endl;
        return;
    }
    for (int i=0; i < PQnfields(res); i++)
        cout << PQfname(res, i) << "\t";
    cout << "\n";
    for (int i=0; i < PQntuples(res); i++) {
        for (int j=0; j < PQnfields(res); j++)
            cout << PQgetvalue(res, i, j) << "\t";
        cout << "\n";
    }
    cout << endl;
    PQclear(res);
}

bool DatabaseConnection::commandQuery(const string& query) {
    PGresult* res = PQexec(conn, stoca(query));
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cout << "command query: " << query << "\ncommand failed." << endl;
        return false;
    }
    cout << "query executed" << endl;
    PQclear(res);
    return true;
}


