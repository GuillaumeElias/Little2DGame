#include "Item.h"

Item::Item(SDL_Renderer* gRenderer, SDL_Window* gWindow, int posX, int posY, LTextureFactory* lTextureFactory, int id,
           PlayerInventory* inventory, bool (PlayerInventory::*addItem) (int, Item*))
 : GameObject(gRenderer, gWindow, posX, posY, lTextureFactory), id(id), playerInventory(inventory), addItemFct(addItem){
    textureName << "items/" << id << ".png";
    pickedUp = false;
}

Item::~Item(){

}

void Item::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    if(!pickedUp){
        GameObject::render(gRenderer, mapVisibleLevel);
    }
}

void Item::renderInInventory(int x, int y){
    gTexture->render(x, y);
}


void Item::onCollision(){
    if(((playerInventory)->*(addItemFct))(id, this)){
        pickedUp = true;
        dead = true;
        gTexture->setInGameViewPort(false);

        if(id == 2){ //disables super jump by default
            setDisabled(true);
        }
    }
}
int Item::move(PlayerPosition* playerPos){
    return 0;
}

std::string Item::getTextureName(){
    return textureName.str();
}

void Item::rebirth(){
    if(!pickedUp){
        GameObject::rebirth();
    }
}

void Item::setDisabled(bool dis){
    disabled = dis;

    if(disabled){
        gTexture->setColor(0x55, 0x55, 0x5);
    }else{
        gTexture->setColor(0xFF, 0xFF, 0xFF);
    }
}
