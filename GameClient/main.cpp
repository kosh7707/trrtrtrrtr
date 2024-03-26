#include "ClientCore.h"
#include <vector>
#include <sstream>
using namespace std;

vector<string> split(const string& input, char delimiter) {
    vector<string> result;
    stringstream ss(input);
    string token;
    while (getline(ss, token, delimiter))
        result.push_back(token);
    return result;
}

int main() {
    try {
        ClientCore clientCore;

        // TODO: 로그인이 되어있지 않을 시, 로그인 응답을 제외한 메시지를 버려야 함.
        while (!clientCore.getIsLogin()) {
            string id, pw;
            cout << "please enter the id:"; cin >> id;
            cout << "please enter the pw:"; cin >> pw;
            clientCore.login(id, pw);
        }

        cin.ignore();
        clientCore.run();
        while (clientCore.getIsLogin()) {
            string msg; getline(cin, msg);
            if (msg[0] != '!') clientCore.sendChat(msg);
            else {
                vector<string> command = split(msg.substr(1), ' ');
                clientCore.sendCommand(command);
            }
        }

    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 0;
    }
}
