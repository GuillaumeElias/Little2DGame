#include "PlayerInventory.h"

PlayerInventory::PlayerInventory(){

}

PlayerInventory::~PlayerInventory(){

}

bool PlayerInventory::addItem(int id, Item* item){
    if(hasItem(id)){
        return false; //no doubles !
    }
    std::cout << "add item inventory : " << id << std::endl;
    items[id] = item;
    return true;
}

bool PlayerInventory::removeItem(int id){
    if(!hasItem(id)){
        return false;
    }
    items.erase(id);
    return true;
}

bool PlayerInventory::hasItem(int id){
    return items.count(id) > 0;
}

const std::map<int, Item *> PlayerInventory::getItems(){
    return items;
}
