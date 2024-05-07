#include "ServerCore.h"
#include "EventHandler.h"
#include "Constants/Constant.h"
#include "PostgreSQL.h"
#include "DAO/InventoryDAO.h"
#include "Model/Item.h"

int main() {
    try {
        Constant constant;

        // TODO: Item의 score를 다른 곳에서 loading해야함.
        Item::idToScore = {
                {1, 1}, {2, 2}, {3, 3}
        };

        std::shared_ptr<IObserver> observer(new Observer());
        std::shared_ptr<IDatabase> db(new PostgreSQL(constant.getConnInfo()));
        std::shared_ptr<AccountDAO> accountDao(new AccountDAO(db));
        std::shared_ptr<InventoryDAO> inventoryDao(new InventoryDAO(db));
        std::shared_ptr<AccountService> accountService(new AccountService(accountDao));
        std::unique_ptr<IEventHandler> eventHandler(new EventHandler(std::dynamic_pointer_cast<Observer>(observer), accountService));

        ServerCore sc(true, std::move(eventHandler), constant.getServerIP(), constant.getServerPort());
        sc.attachSocketObserver(std::dynamic_pointer_cast<Observer>(observer));

        sc.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
}
