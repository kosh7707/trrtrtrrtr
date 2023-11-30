#include "ClientCore.h"

using namespace std;

int main() {
    unsigned int tid;

    ClientCore clientCore;
    clientCore.login();

    clientCore.run();
    SOCKET sc = clientCore.getSocket();
    while (clientCore.getIsLogin()) {
        string msg; cin >> msg;
        send(sc, msg.c_str(), sizeof(msg), 0);
        if (msg == "exit") break;
    }
    return 0;
}
