#include "ClientCore.h"
#include "DatabaseConnection.h"

using namespace std;

int main() {
    ClientCore clientCore;
    clientCore.login();

    IDatabaseConnection& dc = DatabaseConnection::getInstance();
    if (!dc.isConnected()) {
        cout << "Connection to database failed: " << endl;
        exit(1);
    }
    cout << "Connection to database succeed." << endl;

    clientCore.run();

    cin.ignore();
    while (clientCore.getIsLogin()) {
        string msg; getline(cin, msg);
        clientCore.chat(msg);
    }
}
