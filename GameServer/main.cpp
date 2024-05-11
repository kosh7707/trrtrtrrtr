#include "ServerCore.h"
#include "EventHandler.h"
#include "Constants/Constant.h"
#include "PostgreSQL.h"
#include "DAO/InventoryDAO.h"
#include "DAO/AuctionDAO.h"
#include "Service/AuctionService.h"

int main() {
    try {
        Constant constant;

        std::shared_ptr<IObserver> observer(new Observer());

        std::shared_ptr<IDatabase> db(new PostgreSQL(constant.getConnInfo()));
        std::shared_ptr<AccountDAO> accountDao(new AccountDAO(db));
        std::shared_ptr<InventoryDAO> inventoryDao(new InventoryDAO(db));
        std::shared_ptr<AuctionSystem::AuctionDAO> auctionDao(new AuctionSystem::AuctionDAO(db));

        std::shared_ptr<AccountService> accountService(new AccountService(accountDao));
        std::shared_ptr<InventoryService> inventoryService(new InventoryService(accountDao, inventoryDao));
        std::shared_ptr<AuctionSystem::AuctionService> auctionService(new AuctionSystem::AuctionService(auctionDao));

        std::unique_ptr<IEventHandler> eventHandler(new EventHandler(std::dynamic_pointer_cast<Observer>(observer), accountService, inventoryService, auctionService));

        ServerCore sc(true, std::move(eventHandler), constant.getServerIP(), constant.getServerPort());
        sc.attachSocketObserver(std::dynamic_pointer_cast<Observer>(observer));

        sc.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
}
