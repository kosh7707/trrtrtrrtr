#ifndef GAMESERVER_ITEM_H
#define GAMESERVER_ITEM_H


#include <unordered_map>

class Item {
public:
    // TODO: Item의 score를 다른 곳에서 loading해야함.
    static inline std::unordered_map<int, int> idToScore {
            {1, 1}, {2, 2}, {3, 3}
    };

    Item() {}
    Item(int itemId, int score, int quantity) : item_id(itemId), score(score), quantity(quantity) {}

    int getItemId() const {
        return item_id;
    }

    int getScore() const {
        return score;
    }

    int getQuantity() const {
        return quantity;
    }

    void setItemId(int _itemId) {
        item_id = _itemId;
    }

    void setScore(int _score) {
        score = _score;
    }

    void setQuantity(int _quantity) {
        quantity = _quantity;
    }

private:
    int item_id;
    int score;
    int quantity;
};


#endif //GAMESERVER_ITEM_H
