#ifndef DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H
#define DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H


#include <string>

using namespace std;

class ClientConstant {
public:
    const string& getServerIP() const {
        return serverIP;
    }
    const int getServerPort() const {
        return serverPort;
    }
    const string &getId() const {
        return db_id;
    }
    const string& getPw() const {
        return db_pw;
    }
    void setId(const string& id) {
        db_id = id;
    }
    void setPw(const string& pw) {
        db_pw = pw;
    }
    const string& getConnInfo() {
        connInfo = "postgresql://postgres@localhost?port=5432&dbname=databasetermproject&user=" + db_id + "&password=" + db_pw;
        return connInfo;
    }
private:
    string  serverIP = "127.0.0.1";
    int     serverPort = 7707;
    string  db_id;
    string  db_pw;
    string  connInfo;
};


#endif //DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H
