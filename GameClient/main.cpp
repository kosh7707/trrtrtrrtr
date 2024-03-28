#include "ClientCore.h"

int main() {
    try {

        ClientCore cc;
        cc.run();

    } catch(const std::exception& e) {
        std::cerr << e.what()<< std::endl;
        return 0;
    }
}
