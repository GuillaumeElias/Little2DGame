#ifndef PLAYERINVENTORY_H
#define PLAYERINVENTORY_H

#include <map>
#include <Item.h>

/**
Contains items that the player has picked up.
*/
class PlayerInventory
{
    public:
        PlayerInventory();
        virtual ~PlayerInventory();

        bool addItem(int id, Item* item);
        bool removeItem(int id);
        bool hasItem(int id);
        const std::map<int, Item *> getItems();
    protected:
    private:
       std::map<int, Item *> items;
};

#endif // PLAYERINVENTORY_H
