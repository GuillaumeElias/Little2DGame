#include "Objects/BeastObject.h"

BeastObject::BeastObject(SDL_Renderer* renderer, SDL_Window* window, BottomBar* bottomBar, int posX, int posY, LTextureFactory* lTextFact,
                    Map* map, bool (Map::*checkCol) (const SDL_Rect &, bool))
: GameObject(renderer, window, posX, posY, lTextFact), checkCollision(checkCol), map(map), initPosX(posX), initPosY(posY), bottomBar(bottomBar)
{

}

BeastObject::~BeastObject()
{
    //dtor
}


void BeastObject::init(){
    GameObject::init();

    life = BEAST_LIFE;
    direction = LEFT;

    width = BEAST_WIDTH;

    //initialize the clip rectangle
    clip.y = 0;
    clip.w = width;
    clip.h = gTexture->getHeight();

    //initialize the collider rectangle size
    collider.w = width;
    collider.h = gTexture->getHeight();

    //initialize the player collider rectangle
    _playerRect.w = PLAYER_WIDTH;
    _playerRect.h = PLAYER_HEIGHT;

    animTimer.start();
}

std::string BeastObject::getTextureName(){
    return "beast.png";
}

int BeastObject::move(PlayerPosition* playerPos){
    if(dying){
        return 0;
    }

    collider.y = posY;
    _playerRect.y = playerPos->y;

    if(direction == LEFT){ //if going left

        //add the displacement of the beast to the player to anticipate next position
        _playerRect.x = playerPos->x + BEAST_DISPLACEMENT;
        collider.x = posX - BEAST_DISPLACEMENT;

        //check if wall or player is in the way
        if( posX <= 0 || ((map)->*(checkCollision))(collider, true) || GameObject::checkCollision(_playerRect, false)){
            direction = RIGHT; //reverse direction
        }else{
            posX = collider.x;
        }
    }else{ //RIGHT
        _playerRect.x = playerPos->x - BEAST_DISPLACEMENT;
        collider.x = posX + BEAST_DISPLACEMENT;

        if(((map)->*(checkCollision))(collider, true) || GameObject::checkCollision(_playerRect, false)){
            direction = LEFT;
        }else{
            posX = collider.x;
        }
    }

    return 0;
}

void BeastObject::onCollision(){
    if(!hitTimer.isStarted()){ //if beast is not being hit
        hitTimer.start();
        bottomBar->takeHit(1);
    }else if(hitTimer.getTicks() > BLOB_TOUCH_RESET){ //reset the hit timer if time has passed
        hitTimer.stop();
        hitTimer.start();
        bottomBar->takeHit(1);
    }
}

int BeastObject::onHit(BulletType bulletType){
    if(life <= 1){
        dying = true;
        return 3;
    }else{
        life--; //TODO depends on bullet type
        return 0;
    }
}

void BeastObject::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    if(dead){
        animTimer.stop(); //stops the anim timer if beast is dead
        return;
    }

    //go to next frame if anim timer > frame time
    if(animTimer.isStarted() && animTimer.getTicks() > BEAST_ANIM_FRAME_TIME){
        curFrame = (curFrame + 1) % BEAST_ANIM_NB_FRAMES;
        animTimer.reset();
    }

    if(direction == LEFT){ //first frames if going left
        clip.x = 0 + curFrame * width;
    }else{ //last frames if going right
        clip.x = width * BEAST_ANIM_NB_FRAMES + curFrame * width;
    }

    GameObject::render(gRenderer, mapVisibleLevel);
}

void BeastObject::rebirth(){
    life = BEAST_LIFE;
    posX = initPosX;
    posY = initPosY;
    animTimer.start();
    GameObject::rebirth();
}
