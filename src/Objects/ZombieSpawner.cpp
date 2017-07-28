#include "ZombieSpawner.h"
#include "Map.h"

ZombieSpawner::ZombieSpawner(SDL_Renderer* gRenderer, SDL_Window* gWindow, BottomBar* bottomB, int posX, int posY, LTextureFactory* lTextFact,
                              Map* map, int direction )
 : GameObject(gRenderer, gWindow, posX, posY, lTextFact), bottomBar(bottomB), map(map), initPosX(posX), initPosY(posY)
{
    if(direction > 0 && direction <= 1){
        this->direction = static_cast<Direction>(direction);
    }
}

ZombieSpawner::~ZombieSpawner()
{
    //dtor
}

void ZombieSpawner::init(){
    GameObject::init();

    width = width / ZOMBIE_SPAWNER_ANIMATION_FRAMES / 2;

    //initialize the clip rectangle
    clip.y = 0;
    clip.w = width;
    clip.h = gTexture->getHeight();

    //initialize the collider rectangle size
    collider.w = width;
    collider.h = gTexture->getHeight();

    animTimer.start();
    spawnTimer.start();
}

int ZombieSpawner::move(PlayerPosition *){
    //go to next frame if anim timer > frame time
    if(animTimer.isStarted() && animTimer.getTicks() > ZOMBIE_SPAWNER_ANIMATION_TIME){
        curFrame = (curFrame + 1) % ZOMBIE_SPAWNER_ANIMATION_FRAMES;
        animTimer.reset();
    }

    if(direction == LEFT){ //first frames if going left
        clip.x = 0 + curFrame * width;
    }else{ //last frames if going right
        clip.x = width * ZOMBIE_SPAWNER_ANIMATION_FRAMES + curFrame * width;
    }

    //spawn zombie if number of alive zombies spawned < max
    if(spawnTimer.getTicks() > ZOMBIE_SPAWNER_RATE){
        if(map->countLiveZombiesForSpawner(this) < ZOMBIE_SPAWNER_MAX_LIVE_SPAWNED_ZOMBIES){
            spawnZombie();
        }
        spawnTimer.reset();
    }

    return 0;
}

void ZombieSpawner::spawnZombie(){
    //direction == left
    int zombieX = posX - ZOMBIE_WIDTH;
    int zombieY = posY + 8;

    if(direction == RIGHT){
        zombieX = posX + width;
    }

    ZombieObject * zombie = new ZombieObject(gRenderer, gWindow, bottomBar, zombieX, zombieY, lTextureFactory, 300);
    zombie->init();
    zombie->setDirection(direction);
    zombie->setSpawner(this);
    map->getGameObjects()->push_back(zombie);
}

void ZombieSpawner::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    if(dead){
        spawnTimer.stop(); //stops the anim timer if beast is dead
        return;
    }

    gTexture->render( posX - mapVisibleLevel.x, posY, &clip);
}

void ZombieSpawner::onCollision(){
    //do nothing
}

std::string ZombieSpawner::getTextureName(){
    return "zombieSpawner.png";
}
