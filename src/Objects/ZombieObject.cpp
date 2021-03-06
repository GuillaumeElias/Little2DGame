#include "Objects/ZombieObject.h"
#include "SoundEngine.h"

ZombieObject::ZombieObject(SDL_Renderer* renderer, SDL_Window* window, BottomBar* bottomBar, int pX, int pY, LTextureFactory* lTextFact, int distance)
: GameObject(renderer, window, pX, pY, lTextFact), initPosX(pX), distance(distance), bottomBar(bottomBar), spawner(nullptr)
{

}

void ZombieObject::init(){
    GameObject::init();

    //initialize life and direction
    life = ZOMBIE_LIFE;
    direction = LEFT;

    width = ZOMBIE_WIDTH;
    displacement = ZOMBIE_INIT_DISPLACEMENT;

    clip.y = 0;
    clip.w = width;
    clip.h = gTexture->getHeight();
}

ZombieObject::~ZombieObject()
{
    //dtor
}

std::string ZombieObject::getTextureName(){
    return "zombie.png";
}

int ZombieObject::move(PlayerPosition* playerPos){


    if(direction == LEFT){
        if(initPosX - posX > distance){ //if exceeded the distance
            direction = RIGHT;
        }else{
            posX -= displacement;
        }
    }else{ //RIGHT
        if(posX - initPosX > distance){
            direction = LEFT;
        }else{
            posX += displacement;
        }
    }

    return 0;
}

void ZombieObject::onCollision(){
    if(!hitTimer.isStarted()){ //if player is being hit
        hitTimer.start();
        bottomBar->takeHit(1);
    }else if(hitTimer.getTicks() > BLOB_TOUCH_RESET){ //if hit timer finished
        hitTimer.stop();
        hitTimer.start();
        bottomBar->takeHit(1);
    }
}

int ZombieObject::onHit(BulletType bulletType){
    if(life <= 1){
        dying = true;
        SoundEngine::getInstance()->soundEvent(ZOMBIE_DEATH);
        return 2;
    }else{
        life--; //TODO depends on bullet type
        return 0;
    }
}

void ZombieObject::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    if(dead){
        return;
    }

    if(direction == RIGHT){
        clip.x = 0;
    }else{
        clip.x = width;
    }

    GameObject::render(gRenderer, mapVisibleLevel);
}

void ZombieObject::rebirth(){
    life = ZOMBIE_LIFE;
    posX = initPosX;
    GameObject::rebirth();
}

void ZombieObject::setDirection(Direction direction){
    this->direction = direction;
}

ZombieSpawner * ZombieObject::getSpawner(){
    return spawner;
}

void ZombieObject::setSpawner(ZombieSpawner * zombieSpawner){
    this->spawner = zombieSpawner;
}

void ZombieObject::setDisplacement(int displacement){
    this->displacement = displacement;
}
