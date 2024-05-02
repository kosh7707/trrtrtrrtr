#include "ServerCore.h"
#include "ServerConstant.h"
#include "../Database/DatabaseConnection.h"

int main() {
    try {
        IDatabaseConnection& dc = DatabaseConnection::getInstance();

        ServerCore sc;
        sc.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
}
