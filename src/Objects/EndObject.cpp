#include "Objects/EndObject.h"

EndObject::EndObject(SDL_Renderer* renderer, SDL_Window* window, BottomBar* bottomBar, int pX, int pY, LTextureFactory* lTextFact, bool deactivate)
 : GameObject(renderer, window, pX, pY, lTextFact), bottomBar(bottomBar), deactivated(deactivate){
    //Set top left sprite
    gSpriteClips[0].x = 0;
    gSpriteClips[0].y = 0;
    gSpriteClips[0].w = 41;
    gSpriteClips[0].h = 41;

    //Set top right sprite
    gSpriteClips[1].x = 41;
    gSpriteClips[1].y =  0;
    gSpriteClips[1].w = 41;
    gSpriteClips[1].h = 41;
}

EndObject::~EndObject()
{
    delete gAnim;
}

int EndObject::move(PlayerPosition* playerPos){
    if(deactivated){
        return 0;
    }

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

std::string EndObject::getTextureName(){
    return "end.png";
}

void EndObject::setDeactivated(bool deactivated){
    this->deactivated = deactivated;
}

void EndObject::onCollision(){
    if(!deactivated){
        bottomBar->levelCompleted();
    }
}

void EndObject::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    //selects current clip and render it
    SDL_Rect* currentClip = &gSpriteClips[ frame ];
    gTexture->render( posX - mapVisibleLevel.x, posY, currentClip );
}
