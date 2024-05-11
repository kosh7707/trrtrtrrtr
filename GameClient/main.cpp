#include <iostream>
#include "Constants/Constant.h"
#include "ServerCore.h"
#include "EventHandler.h"

int main() {
    try {
        Constant constant;
        std::unique_ptr<IEventHandler> eventHandler(new EventHandler());

        ServerCore sc(false, std::move(eventHandler), constant.getMainServerIP(), constant.getMainServerPort());
        if (sc.connect(constant.getChatServerIp(), constant.getChatServerPort()))
            std::cout << "connected to chat server" << std::endl;

        sc.run();

    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
}
