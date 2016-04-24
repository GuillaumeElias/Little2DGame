#include "BallisticEngine.h"

BallisticEngine::BallisticEngine(SDL_Renderer* gRenderer){
    bulletTexture = new LTexture(gRenderer, true);

    //load bullet texture
    if( !bulletTexture->loadFromFile("img/bullet.png", {255, 255, 255} ) ){
        printf( "Failed to load bullet texture!\n" );
        return;
    }
}

void BallisticEngine::fireBullet(const Position position, Direction direction){
    BulletPosition bulletPos;
    bulletPos.y = position.y + BULLET_TOP_MARGIN; //set bullet y
    bulletPos.direction = direction;

    bulletPos.x = position.x;
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

     for(int i=0; i<bullets.size(); i++){ //for each active bullet
        collider.y = bullets[i].y;

        int bulletMove = BULLET_DISPLACEMENT; //initialize bullet move

        while(true){
            if(bullets[i].direction == RIGHT){
                collider.x = bullets[i].x + bulletMove; //add bullet move to collider

                if(bullets[i].x >= map->getLevelWidth() || map->checkCollision(collider, true)){
                    if(bulletMove < 2){ //if bulletMove can not be smaller
                        //remove bullet and exit loop
                        bullets.erase(bullets.begin() + i);
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
                        bullets.erase(bullets.begin() + i);
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

        //check collision against every game object
        std::vector<GameObject*>* gameObjects = map->getGameObjects();
        for(int j=0; j<gameObjects->size(); j++){
            if(gameObjects->at(j)->checkCollision(collider, true)){
                bullets.erase(bullets.begin() + i);
                earnedPoints += gameObjects->at(j)->onHit(PLAYER_BULLET_1); //TODO get bullet type from bullet
            }
        }

        if(earnedPoints>0){
            map->getBottomBar()->addPoints(earnedPoints);
        }

     }
}

void BallisticEngine::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    //render all bullets
    for(int i=0; i<bullets.size(); i++){
        bulletTexture->render(bullets[i].x - mapVisibleLevel.x, bullets[i].y);
    }
}

BallisticEngine::~BallisticEngine()
{
    //dtor
}
