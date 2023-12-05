#include "ClientCore.h"

using namespace std;

int main() {
    ClientCore clientCore;
    clientCore.login();
    clientCore.run();

    cin.ignore();
    while (clientCore.getIsLogin()) {
        string msg; getline(cin, msg);
        clientCore.chat(msg);
    }
}
