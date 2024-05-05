#ifndef DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H
#define DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H

#include <string>

class Constant {
public:
    const std::string getMainServerIP() const {
        return MainServerIP;
    }
    const int getMainServerPort() const {
        return MainServerPort;
    }
private:
    const std::string   MainServerIP   = "127.0.0.1";
    const int           MainServerPort = 7707;
};


#endif //DATABASETERMPROJECTCLIENT_CLIENTCONSTANT_H
