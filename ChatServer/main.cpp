#include "ServerCore.h"
#include "Constant.h"
#include "IDatabase.h"
#include "PostgreSQL.h"
#include "EventHandler.h"
#include "Observer.h"
#include <iostream>

int main() {
    try {
        Constant constant;

        std::shared_ptr<IDatabase> db(new PostgreSQL(constant.getConnInfo()));

        std::shared_ptr<IObserver> observer(new Observer());

        std::unique_ptr<IEventHandler> eventHandler(new EventHandler(std::dynamic_pointer_cast<Observer>(observer)));

        ServerCore sc(true, std::move(eventHandler), constant.getServerIP(), constant.getServerPort());
        if (sc.connect(constant.getMainServerIP(), constant.getMainServerPort()))
            std::cout << "connected to main server" << std::endl;
        sc.attachSocketObserver(observer);

        sc.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
}
