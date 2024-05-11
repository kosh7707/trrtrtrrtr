#ifndef GAMESERVER_ITEMCONSTANT_H
#define GAMESERVER_ITEMCONSTANT_H


#include <unordered_map>
#include <string>

class ItemConstant {
public:
    static inline std::string getItemName(int item_id) {
        return item_name[item_id];
    }
    static inline int getItemScore(int item_id) {
        return item_score[item_id];
    }
private:
    static inline std::unordered_map<int, std::string> item_name = {
            {1, "test_item1"},
            {2, "test_item2"},
            {3, "test_item3"},
    };
    static inline std::unordered_map<int, int> item_score = {
            {1, 1},
            {2, 2},
            {3, 3},
    };
};


#endif //GAMESERVER_ITEMCONSTANT_H
