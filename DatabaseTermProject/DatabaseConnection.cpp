#include "DatabaseConnection.h"

bool DatabaseConnection::isConnected() {
    return PQstatus(conn) == CONNECTION_OK;
}

void DatabaseConnection::connectionClose() {
    PQfinish(conn);
}

DatabaseConnection::DatabaseConnection() {
    const string connInfo = serverConstant.getConnInfo();
    conn = PQconnectdb(connInfo.c_str());
    if (PQstatus(conn) != CONNECTION_OK)
        PQfinish(conn);
}

PGresult* DatabaseConnection::selectQuery(const std::string& query) {
    PGresult* res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cout << "select query: " << query << "\nselect failed." << endl;
        return nullptr;
    }
    return res;
}

bool DatabaseConnection::commandQuery(const string& query) {
    PGresult* res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cout << "command query: " << query << "\ncommand failed." << endl;
        return false;
    }
    PQclear(res);
    return true;
}

const PGconn* DatabaseConnection::getConn() const {
    return conn;
}


