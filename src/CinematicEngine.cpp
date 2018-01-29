#include "CinematicEngine.h"
#include <iostream>
#include <sstream>

CinematicEngine::CinematicEngine(SDL_Renderer* renderer, SDL_Window* window) : gRenderer(renderer), gWindow(window) {

    sceneTextures.reserve(totalScenes);
    for(int i = 0; i<totalScenes; i++){
        sceneTextures[i] = new LTexture(gRenderer, false);

        std::ostringstream path;
        path << "img/intro/" << i << ".png";

        if( !sceneTextures[i]->loadFromFile(path.str(), { 0xFF, 0xFF, 0xFF } ) ){
            printf( "Failed to load pazook game card selection texture!\n" );
            return;
        }
        sceneTextures[i]->setScaling(i+1);
    }

    //Set top left sprite
    gSpriteClips[ 0 ].x = 0;
    gSpriteClips[ 0 ].y = 0;
    gSpriteClips[ 0 ].w = 30;
    gSpriteClips[ 0 ].h = 55;

    //Set top right sprite
    gSpriteClips[ 1 ].x = 30;
    gSpriteClips[ 1 ].y =  0;
    gSpriteClips[ 1 ].w = 30;
    gSpriteClips[ 1 ].h = 55;

    //Set bottom left sprite
    gSpriteClips[ 2 ].x = 60;
    gSpriteClips[ 2 ].y = 0;
    gSpriteClips[ 2 ].w = 30;
    gSpriteClips[ 2 ].h = 55;

    //Set bottom right sprite
    gSpriteClips[ 3 ].x = 90;
    gSpriteClips[ 3 ].y = 0;
    gSpriteClips[ 3 ].w = 30;
    gSpriteClips[ 3 ].h = 55;

    state = INTRO;
    frame = 0;
    scene = 0;
}

CinematicEngine::~CinematicEngine(){
    for(int i = 0; i<totalScenes; i++){
        delete sceneTextures[scene];
    }
    sceneTextures.clear();
}

//NOTE: I had no inspiration whatsoever for the intro scene so the following code is just random mambo-jumbo therefore creating random visuals
void CinematicEngine::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){

    if(state == INTRO && !sceneTimer.isStarted()){
        sceneTimer.start();
    }

    if(sceneTimer.getTicks() > sceneTime){

        if(scene >= totalScenes - 1){
            ending = true;
        }else{
            scene++;
        }

        sceneTimer.reset();
    }

    if(!frameTimer.isStarted()){
        frameTimer.start();
    }

    if(frameTimer.getTicks() > frameTime){
        if(frame >= 3){
            frame = 0;
        }else{
            frame++;
        }
        frameTimer.reset();
    }

    static float scaling = 1;
    static int x, y;

    if(ending){
        if(scaling <= 0){
            state = OFF;
            scene = 0;
            return;
        }else{
            scaling -= 0.2;

            x = (SCREEN_WIDTH / 2)  - (sceneTextures[scene]->getWidth() / 8);
            y = 0;
        }
    }else if(scene > 7){
        scaling = 20;
        x = (SCREEN_WIDTH / 2) - (sceneTextures[scene]->getWidth() / 8);
        y = 0;

        sceneTime = 550;
    }else {
        scaling = sceneTextures[scene]->getScaling() + 0.001;
        x = ((float)sceneTimer.getTicks() / sceneTime) * (SCREEN_WIDTH - sceneTextures[scene]->getWidth() / 4);
        y = sceneTimer.getTicks() / 10 % 10;
    }

    sceneTextures[scene]->setScaling( scaling );

    SDL_Rect* currentClip = &gSpriteClips[ frame ];
    sceneTextures[scene]->render(x , y , currentClip );
}

CinematicState CinematicEngine::getState() const {
    return state;
}

