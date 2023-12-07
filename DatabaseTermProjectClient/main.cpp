#include "ClientCore.h"
#include "DatabaseConnection.h"

using namespace std;

int main() {
    ClientCore clientCore;
    clientCore.login();

    clientCore.connectDB();
    DatabaseConnection& dc = clientCore.getDatabaseConnection();
    if (!dc.isConnected()) {
        cout << "Connection to database failed: " << endl;
        exit(1);
    }
    cout << "Connection to database succeed." << endl;

    clientCore.run();

    cin.ignore();
    while (clientCore.getIsLogin()) {
        string msg; getline(cin, msg);
        if (msg[0] != '!') clientCore.chat(msg);
        else {
            // TODO Ŀ�ǵ� ó�� �ڵ� �ۼ�
            // test ������ �ޱ�� Ŀ�ǵ�
            // �κ��丮 Ȯ�� ��û�� Ŀ�ǵ�
            // ���� ���� Ŀ�ǵ�
            // ��� ��ǰ Ȯ�� Ŀ�ǵ�
            // ��� ���� Ŀ�ǵ�
        }
    }
}
