#include "ServerCore.h"
#include "EventHandler.h"
#include "Constants/Constant.h"
#include "PostgreSQL.h"
#include "DAO/InventoryDAO.h"

int main() {
    try {
        Constant constant;

        std::shared_ptr<IDatabase> db(new PostgreSQL(constant.getConnInfo()));
        std::shared_ptr<AccountDAO> accountDao(new AccountDAO(db));
        std::shared_ptr<InventoryDAO> inventoryDao(new InventoryDAO(db));
        std::shared_ptr<AccountService> accountService(new AccountService(accountDao));
        std::unique_ptr<IEventHandler> eventHandler(new EventHandler(accountService));

        ServerCore sc(true, std::move(eventHandler), constant.getServerIP(), constant.getServerPort());

        sc.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
}
