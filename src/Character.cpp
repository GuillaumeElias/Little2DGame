#include "Character.h"

Character::Character(SDL_Renderer* renderer, SDL_Window* window, int pX, int pY, LTextureFactory* lTextFact, int characterId)
: GameObject(renderer, window, pX, pY, lTextFact), characterId(characterId){
    //define textureName based on character id
    textureName << "characters/" << characterId << ".png";
}

void Character::init(){
    GameObject::init();

    //calculates width based on texture width and number of frames in it
    width = gTexture->getWidth() / CHARACTER_NB_SPRITES;

    //initializes clip rectangle
    clip.x = 0;
    clip.y = 0;
    clip.w = width;
    clip.h = gTexture->getHeight();
}

Character::~Character(){
    //dtor
}

void Character::renderInDialog(SDL_Renderer* gRenderer){
    move(NULL); //makes character move with no position (because not in game)
    clip.x = currentSpriteNb * clip.w;

    gTexture->render( DIALOG_LEFT_MARGIN, DIALOG_CHARACTER_TOP_MARGIN - height/2, &clip);
}

void Character::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    clip.x = currentSpriteNb * clip.w;

    gTexture->render( posX - mapVisibleLevel.x, posY, &clip);
}

std::string Character::getTextureName(){
    return textureName.str();
}

int Character::move(PlayerPosition* playerPos){
    if(!moveTimer.isStarted()){
        moveTimer.start();
    }
    if(moveTimer.getTicks() > CHARACTER_MOVE_TIME){
        moveTimer.stop();
        if(currentSpriteNb >= CHARACTER_NB_SPRITES - 1){
            currentSpriteNb = 0;
        }else{
            currentSpriteNb++;
        }
        moveTimer.start();
    }
    return 0;
}

void Character::onCollision(){

}

int Character::getId(){
    return characterId;
}
