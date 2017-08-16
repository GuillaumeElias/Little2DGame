#include "BallisticEngine.h"
#include <set>

BallisticEngine::BallisticEngine(SDL_Renderer* gRenderer, BottomBar * bottomBa) : bottomBar(bottomBa) {
    playerBulletTexture = new LTexture(gRenderer, true);

    //load player bullet texture
    if( !playerBulletTexture->loadFromFile("img/playerBullet.png", {255, 255, 255} ) ){
        printf( "Failed to load player bullet texture!\n" );
        return;
    }

    enemyBulletTexture = new LTexture(gRenderer, true);

    //load enemy bullet texture
    if( !enemyBulletTexture->loadFromFile("img/enemyBullet.png", {255, 255, 255} ) ){
        printf( "Failed to load enemy bullet texture!\n" );
        return;
    }
}

void BallisticEngine::fireBullet(const Position position, Direction direction, bool enemy){
    BulletPosition bulletPos;
    bulletPos.x = position.x;
    bulletPos.y = position.y;
    bulletPos.direction = direction;
    bulletPos.enemy = enemy;

    if(direction == RIGHT){
        bulletPos.x += PLAYER_WIDTH; //if going right move bullet on the other side of the player
    }

    bullets.push_back(bulletPos);
}

void BallisticEngine::move(Map* map){
    //initialize common collider
    SDL_Rect collider;
    collider.w = 1;
    collider.h = 1;

    std::set<BulletPosition> bulletsToErase;

    for(int i=0; i<bullets.size(); i++){ //for each active bullet
        collider.y = bullets[i].y;

        int bulletMove = BULLET_DISPLACEMENT; //initialize bullet move

        while(true){
            if(bullets[i].direction == RIGHT){
                collider.x = bullets[i].x + bulletMove; //add bullet move to collider

                if(bullets[i].x >= map->getLevelWidth() || map->checkCollision(collider, true)){
                    if(bulletMove < 2){ //if bulletMove can not be smaller
                        //remove bullet and exit loop
                        bulletsToErase.insert(bullets[i]);
                        break;
                    }else{ //reduce bulletMove and try again
                        bulletMove /= 2;
                        continue;
                    }

                }else{ //no collision
                    //move bullet and exit loop
                    bullets[i].x += bulletMove;
                    break;
                }
            }else{ //direction == LEFT

                collider.x = bullets[i].x - bulletMove;

                if(bullets[i].x <= 0 || map->checkCollision(collider, true)){
                    if(bulletMove < 2){
                        bulletsToErase.insert(bullets[i]);
                        break;
                    }else{
                        bulletMove /= 2;
                        continue;
                    }
                }else{
                    bullets[i].x -= bulletMove;
                    break;
                }
            }


        }

        int earnedPoints = 0;

        if(bullets[i].enemy){
            if(bullets[i].x >= playerPosition->x && bullets[i].x <= playerPosition->x + PLAYER_WIDTH &&
               bullets[i].y + BULLET_COLL_MARGIN_Y >= playerPosition->y &&
               bullets[i].y + BULLET_COLL_MARGIN_Y <= playerPosition->y + PLAYER_HEIGHT){
                    bottomBar->takeHit(ENEMY_BULLET_DAMAGE);
                    bulletsToErase.insert(bullets[i]);
               }
        }else{ //player's bullet
             //check collision against every game object
            std::vector<GameObject*>* gameObjects = map->getGameObjects();
            for(int j=0; j<gameObjects->size(); j++){
                if(gameObjects->at(j)->checkCollision(collider, true)){
                    bulletsToErase.insert(bullets[i]);
                    earnedPoints += gameObjects->at(j)->onHit(PLAYER_BULLET_1); //TODO get bullet type from bullet
                }
            }

            if(earnedPoints>0){
                map->getBottomBar()->addPoints(earnedPoints);
            }
        }

    }

    for(BulletPosition bulletToErase : bulletsToErase){
        for(int i=0; i<bullets.size(); i++){
            if(bullets[i] == bulletToErase){
                bullets.erase(bullets.begin() + i);
                break;
            }
        }
    }

}

void BallisticEngine::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    //render all bullets
    for(int i=0; i<bullets.size(); i++){
        if(bullets[i].enemy){
            enemyBulletTexture->render(bullets[i].x - mapVisibleLevel.x, bullets[i].y);
        }else{
            playerBulletTexture->render(bullets[i].x - mapVisibleLevel.x, bullets[i].y);
        }
    }
}

void BallisticEngine::clearBullets(){
    bullets.clear();
}

BallisticEngine::~BallisticEngine()
{
    delete playerBulletTexture;
    delete enemyBulletTexture;
}

void BallisticEngine::setPlayerPosition(PlayerPosition * playerPosition){
    this->playerPosition = playerPosition;
}
