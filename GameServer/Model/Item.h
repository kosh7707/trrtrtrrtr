#ifndef GAMESERVER_ITEM_H
#define GAMESERVER_ITEM_H


class Item {
public:
    Item(int item_id) : item_id(item_id) {}

    int getItemId() const {
        return item_id;
    }

private:
    const int item_id;
};


#endif //GAMESERVER_ITEM_H
