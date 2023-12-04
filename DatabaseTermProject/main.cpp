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
    cout << "Connection to database succeed." << endl;

    ServerCore sc;
    sc.run();

    dc.connectionClose();
    return 0;
}
