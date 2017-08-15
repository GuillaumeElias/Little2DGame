#include "Objects/BlobObject.h"

BlobObject::BlobObject(SDL_Renderer* renderer, SDL_Window* window, BottomBar* bottomBar, int pX, int pY, LTextureFactory* lTextFact) : GameObject(renderer, window, pX, pY, lTextFact), initPosY(pY), bottomBar(bottomBar)
{
    up = true;
    life = BLOB_LIFE;
}

BlobObject::~BlobObject()
{

}

std::string BlobObject::getTextureName(){
    return "blob.png";
}

int BlobObject::move(PlayerPosition* playerPos){
    if(dying){
        return 0;
    }

    if(up){ //if moving up
        posY -= 1;
        if(posY < initPosY - BLOB_JUMP_HEIGHT){ //if passed max jump height
            up = false;
        }
        // if player is above, then move player upwards
        if(playerPos->y > 0 && isPlayerAbove(playerPos)){
            playerPos->y--;
        }
    }else{ //if moving down
        posY += 1;
        if(posY >= initPosY){ //if reached initial position
            up = true;
        }
    }
    return 0;
}

void BlobObject::onCollision(){
    if(!hitTimer.isStarted()){ //if player is being hit
        hitTimer.start();
        bottomBar->takeHit(1);
    }else if(hitTimer.getTicks() > BLOB_TOUCH_RESET){ //if hit timer finished
        hitTimer.stop();
        hitTimer.start();
        bottomBar->takeHit(1);
    }
}

int BlobObject::onHit(BulletType bulletType){
    if(life <= 1){
        dying = true;
        return 1;
    }else{
        life--; //TODO depends on bullet type
        return 0;
    }
}

void BlobObject::rebirth(){
    life = BLOB_LIFE;
    GameObject::rebirth();
}
