#include "ServerCore.h"
#include "ServerConstant.h"
#include "DatabaseConnection.h"

using namespace std;

int main() {
    try {
        IDatabaseConnection& dc = DatabaseConnection::getInstance();

        ServerCore sc;
        sc.run();

    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 0;
    }
}
