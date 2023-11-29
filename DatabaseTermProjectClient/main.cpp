#include "ClientCore.h"

using namespace std;

int main() {
    unsigned int tid;

    ClientCore clientCore;
    clientCore.run();
    clientCore.login();

    SOCKET sc = clientCore.getSocket();
    while (true) {
        string msg; cin >> msg;
        send(sc, msg.c_str(), sizeof(msg), 0);
        if (msg == "exit") break;
    }
    return 0;
}
