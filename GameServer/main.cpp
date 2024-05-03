#include "ServerCore.h"
#include "EventHandler.h"
#include "Constants/Constant.h"

int main() {
    try {
        Constant constant;

        std::unique_ptr<IEventHandler> eventHandler(new EventHandler());

        ServerCore sc(constant.getServerIP(), constant.getServerPort(), std::move(eventHandler));

        sc.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
}
