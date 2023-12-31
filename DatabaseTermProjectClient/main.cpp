#include "ClientCore.h"
#include "DatabaseConnection.h"
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
        clientCore.login();
        clientCore.run();

        cin.ignore();
        while (clientCore.getIsLogin()) {
            string msg; getline(cin, msg);
            if (msg[0] != '!') clientCore.handleChat(msg);
            else {
                vector<string> command = split(msg.substr(1), ' ');
                clientCore.handleCommand(command);
            }
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 0;
    }
}
