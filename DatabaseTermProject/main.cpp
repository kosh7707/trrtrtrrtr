#include "ServerCore.h"
#include "ServerConstant.h"
#include "DatabaseConnection.h"

using namespace std;

int main() {
    IDatabaseConnection& dc = DatabaseConnection::getInstance();
    if (!dc.isConnected()) {
        cout << "Connection to database failed: " << endl;
        exit(1);
    }
    cout << "-------------------------------" << endl;
    cout << "Connection to database succeed." << endl;
    cout << "-------------------------------" << endl;

    while (true) {
        cout << "-------------------------------" << endl;
        cout << "SQL test" << endl;
        cout << "Enter the SQL query (u need to input ; at the end of query.) (exit: exit;)\n";
        string query, temp;
        while (temp[temp.length()-1] != ';') {
            getline(cin, temp);
            query += temp + " ";
        }
        if (query == "exit;") break;
        dc.query(query);
    }

    dc.connectionClose();
    return 0;
}
