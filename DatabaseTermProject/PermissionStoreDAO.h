#ifndef DATABASETERMPROJECT_PERMISSIONSTOREDAO_H
#define DATABASETERMPROJECT_PERMISSIONSTOREDAO_H


#include "DatabaseConnection.h"

class PermissionStoreDAO {
public:
    string openPermissionStore();
    bool buyPermission(const string& id, int permission_id);
};


#endif //DATABASETERMPROJECT_PERMISSIONSTOREDAO_H
