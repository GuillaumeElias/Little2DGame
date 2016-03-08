#include "Objects/ElevatorObject.h"

ElevatorObject::ElevatorObject(SDL_Renderer* renderer, SDL_Window* window, int pX, int pY, LTextureFactory* lTextFact, int alt) : GameObject(renderer, window, pX, pY, lTextFact), initPosY(pY)
{
    if(alt > 0){ //if parameter is valid
        altitude=alt;
    }
    up = true;
}

ElevatorObject::~ElevatorObject()
{
    //dtor
}

std::string ElevatorObject::getTextureName(){
    return "elevator.png";
}

int ElevatorObject::move(PlayerPosition* playerPos){
    if(up){
        posY-=1;
        if(posY < initPosY - altitude){
            up = false;
        }
        if(playerPos->y > 0 && isPlayerAbove(playerPos)){
            playerPos->y--;
        }
    }else{
        posY+=1;
        if(posY >= initPosY || isPlayerBelow(playerPos)){
            up = true;
        }
    }
    return 0;
}

void ElevatorObject::onCollision(){
}
