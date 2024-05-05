#include <iostream>
#include "Constant.h"
#include "ServerCore.h"
#include "EventHandler.h"

int main() {
    try {
        Constant constant;
        std::unique_ptr<IEventHandler> eventHandler(new EventHandler());

        ServerCore sc(false, std::move(eventHandler), constant.getMainServerIP(), constant.getMainServerPort());

        sc.run();

    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
}
