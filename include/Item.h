#ifndef ITEM_H
#define ITEM_H

#include <GameObject.h>

class PlayerInventory;

/**
Item to be picked up in game as well as in player's inventory.
*/
class Item : public GameObject
{
    public:
        Item(SDL_Renderer* gRenderer, SDL_Window* gWindow, int posX, int posY, LTextureFactory* lTextureFactory, int id,
              PlayerInventory* inventory, bool (PlayerInventory::*addItem) (int, Item*));
        virtual ~Item();

        virtual void onCollision();
        virtual int move(PlayerPosition* playerPos); //returns 0 or trigger id if one has been hit
        void rebirth();

        void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel);
        void renderInInventory(int x, int y); //render when in inventory (has been picked up)

        virtual std::string getTextureName();
        void setDisabled(bool disabled);
    protected:
    private:
        PlayerInventory* playerInventory;
        bool (PlayerInventory::*addItemFct) (int, Item*);

        std::stringstream textureName;
        int id; //identifier for the item (corresponds to texture name in directory)
        bool pickedUp;
        bool disabled;
};

#endif // ITEM_H
