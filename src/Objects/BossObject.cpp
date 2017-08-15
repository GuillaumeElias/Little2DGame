#include "BossObject.h"
#include "BallisticEngine.h"
#include "Map.h"

BossObject::BossObject(SDL_Renderer* renderer, SDL_Window* window, BottomBar* bottomB, int posX, int posY, LTextureFactory* lTextFact,
                        BallisticEngine * ballisticEng, PlayerPosition * playerPos, Map * map)
: GameObject(renderer, window, posX, posY, lTextFact)
, initPosX(posX), initPosY(posY), bottomBar(bottomB), ballisticEngine(ballisticEng), playerPosition(playerPos), levelMap(map)
{
    //ctor
}

BossObject::~BossObject()
{
    //dtor
}



void BossObject::init(){
    GameObject::init();

    life = BOSS_LIFE;
    direction = LEFT;

    width = width / BOSS_NB_FRAMES / 2;

    //initialize the clip rectangle
    clip.y = 0;
    clip.w = width;
    clip.h = height;

    //initialize the collider rectangle size
    collider.w = width;
    collider.h = gTexture->getHeight();

    normalTexture = gTexture;
    vulnerableTexture = lTextureFactory->fetchTexture("img/bossVulnerable.png", true);

    bossAttackStep = IDLE;

    bottomBar->setBossLife(life);

    animTimer.start();
    idleTimer.start();

}

void BossObject::rebirth(){
    life = BOSS_LIFE;

    posY = initPosY;
    posX = initPosX;
    bossAttackStep = IDLE;
    bottomBar->setBossLife(life);

    gTexture = normalTexture;

    animTimer.start();
    idleTimer.start();
}

std::string BossObject::getTextureName(){
    return "boss.png";
}

int BossObject::move(PlayerPosition* playerPos){
    if(dead || dying){
        return 0;
    }

    collider.y = posY;

    direction = isPlayerAtRight() ? RIGHT : LEFT;

    switch(bossAttackStep){
    case IDLE:
        if(idleTimer.getTicks() > BOSS_IDLE_TIME){
            bossAttackStep = DESCENT;
            idleTimer.stop();
        }
        break;
    case DESCENT:
        if( posY + height >= LEVEL_HEIGHT){
            bossAttackStep = ATTACK;
            attackTimer.start();
        }else{
            posY += BOSS_DESCENT_SPEED;
        }

        break;
    case ATTACK:
        if(!fireTimer.isStarted()){
            fireTimer.start();
        }

        if(fireTimer.getTicks() >= BOSS_FIRE_TIME){
            Position bulPos = Position{ direction==RIGHT ? posX + width : posX, posY + BULLET_BOSS_TOP_MARGIN };
            ballisticEngine->fireBullet(bulPos, direction, true);
            fireTimer.reset();
        }

        if(attackTimer.getTicks() >= BOSS_ATTACK_TIME){
            bossAttackStep = ASCENT;
            attackTimer.stop();
        }

        break;
    case ASCENT:
        if(posY <= initPosY){
            bossAttackStep = VULNERABLE;
            vulnerableTimer.start();
            gTexture = vulnerableTexture;
        }else{
            posY -= BOSS_ASCENT_SPEED;
        }

        break;

    case VULNERABLE:
        if(vulnerableTimer.getTicks() >= BOSS_VULNERABLE_TIME){
            vulnerableTimer.reset();

            gTexture = normalTexture;
            bossAttackStep = IDLE;
            idleTimer.start();
        }
    }

    return 0;
}

void BossObject::onCollision(){
    if(!hitTimer.isStarted()){ //if beast is not being hit
        hitTimer.start();
        bottomBar->takeHit(1);
    }else if(hitTimer.getTicks() > BOSS_TOUCH_RESET){ //reset the hit timer if time has passed
        hitTimer.stop();
        hitTimer.start();
        bottomBar->takeHit(1);
    }
}

int BossObject::onHit(BulletType bulletType){

    if(bossAttackStep == VULNERABLE){
        if(life <= 1){
            dying = true;
            bottomBar->setBossLife(-1);
            activateEnd();
            return 5;
        }else{
            life--; //TODO depends on bullet type
            bottomBar->setBossLife(life);

            return 0;
        }
    }else{
        //TODO display special texture?
        return 0;
    }

}

void BossObject::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    if(dead){
        animTimer.stop(); //stops the anim timer if beast is dead
        return;
    }

    //go to next frame if anim timer > frame time
    if(animTimer.isStarted() && animTimer.getTicks() > BOSS_ANIM_FRAME_TIME){
        curFrame = (curFrame + 1) % BOSS_NB_FRAMES;
        animTimer.reset();
    }

    if(direction == LEFT){ //first frames if going left
        clip.x = 0 + curFrame * width;
    }else{ //last frames if going right
        clip.x = width * BOSS_NB_FRAMES + curFrame * width;
    }

    GameObject::render(gRenderer, mapVisibleLevel);
}

bool BossObject::isPlayerAtRight(){
    return (posX + width / 2) < playerPosition->x;
}

void BossObject::activateEnd(){
    for(GameObject * gameObject : *levelMap->getGameObjects()){
        if(EndObject * endObject = dynamic_cast<EndObject *>(gameObject)){
            endObject->setDeactivated(false);
        }
    }
}
