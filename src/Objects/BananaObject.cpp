#include "Objects/BananaObject.h"
#include "EndObject.h"

BananaObject::BananaObject(SDL_Renderer* renderer, SDL_Window* window, BottomBar* bottomBar, int pX, int pY, LTextureFactory* lTextFact, const std::vector<GameObject*>* theGameObjects)
 : GameObject(renderer, window, pX, pY, lTextFact), bottomBar(bottomBar), gameObjects(theGameObjects){
    //Set top left sprite
    gSpriteClips[0].x = 0;
    gSpriteClips[0].y = 0;
    gSpriteClips[0].w = 30;
    gSpriteClips[0].h = 30;

    //Set top right sprite
    gSpriteClips[1].x = 30;
    gSpriteClips[1].y =  0;
    gSpriteClips[1].w = 30;
    gSpriteClips[1].h = 30;
}

BananaObject::~BananaObject()
{
    delete gAnim;
}

int BananaObject::move(PlayerPosition* playerPos){
    if(!animTimer.isStarted()){
        animTimer.start();
    }
    if(animTimer.getTicks() > END_OBJECT_ANIMATION_FRAME_TIME){
        frame++;

        //Cycle animation
        if( frame >= END_OBJECT_ANIMATION_FRAMES ){
            frame = 0;
        }

        animTimer.reset();
    }
    return 0;
}

std::string BananaObject::getTextureName(){
    return "banana.png";
}

void BananaObject::onCollision(){
    bottomBar->eatBanana();
    this->dead = true;

    if(bottomBar->areAllBananasEaten()){
        for(int i=0; i<gameObjects->size(); i++){
            EndObject * endObject = dynamic_cast<EndObject *> (gameObjects->at(i));
            if(endObject){
                endObject->setDeactivated(false);
            }
        }
    }
}

void BananaObject::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    if(dead){
        return;
    }

    //selects current clip and render it
    SDL_Rect* currentClip = &gSpriteClips[ frame ];
    gTexture->render( posX - mapVisibleLevel.x, posY, currentClip );
}
