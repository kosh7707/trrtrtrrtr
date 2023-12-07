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
            // TODO 커맨드 처리 코드 작성
            // test 아이템 받기용 커맨드
            // 인벤토리 확인 요청용 커맨드
            // 쿼리 실행 커맨드
            // 경매 물품 확인 커맨드
            // 경매 참여 커맨드
        }
    }
}
