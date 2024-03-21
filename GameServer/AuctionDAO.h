#ifndef GAMESERVER_AUCTIONDAO_H
#define GAMESERVER_AUCTIONDAO_H

#include "DatabaseConnection.h"
#include "Client.h"

class AuctionDAO {
public:
    std::string auctionCheck();
    std::vector<std::pair<int, std::string>> outdatedItemCheck(int ClientsCount, std::shared_ptr<Client[]> Clients);
};


#endif //GAMESERVER_AUCTIONDAO_H
