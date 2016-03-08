#include "GameObject.h"

GameObject::GameObject(SDL_Renderer* renderer, SDL_Window* window, int pX, int pY, LTextureFactory* lTextureFact) : gRenderer(renderer), gWindow(window), posX(pX), posY(pY), lTextureFactory(lTextureFact){

}

void GameObject::init(){
    std::ostringstream texturePath;
    texturePath << "img/" << getTextureName();

    std::cout << texturePath.str() << std::endl;

    gTexture = lTextureFactory->fetchTexture(texturePath.str(), true);

    width = gTexture->getWidth();
    height = gTexture->getHeight();
}

GameObject::~GameObject(){
    if(gTexture != NULL){
         gTexture->free();
    }
}

bool GameObject::checkCollision(const SDL_Rect& b, bool checkOnly){
    if(dead){
        return false;
    }

    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = posX;
    rightA = posX + width;
    topA = posY;
    bottomA = posY + height;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if( bottomA <= topB ) {
        return false;
    }

    if( topA >= bottomB ){
        return false;
    }

    if( rightA <= leftB ){
        return false;
    }

    if( leftA >= rightB ) {
        return false;
    }

    if(!checkOnly){
        onCollision();
    }

    //If none of the sides from A are outside B
    return true;
}

void GameObject::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    if(!dead){
         gTexture->render( posX - mapVisibleLevel.x, posY);
    }
}

bool GameObject::isPlayerAbove(PlayerPosition* pPos){
    if(dead){
        return false;
    }

    int pA = pPos->x;
    int pB = pPos->x + PLAYER_WIDTH;
    int oA = posX;
    int oB = posX + gTexture->getWidth();

    if((pA >= oA && pA <= oB) || (pB >= oA && pB <= oB) ){ //if aligned on x
        for(int i=-3; i<2; i++){ /*check y in several points*/
            if((pPos->y + PLAYER_HEIGHT + i) == posY){
                return true;
            }
        }
    }
    return false;
}

bool GameObject::isPlayerBelow(PlayerPosition* pPos){
    if(dead){
        return false;
    }

    int pA = pPos->x;
    int pB = pPos->x + PLAYER_WIDTH;
    int oA = posX;
    int oB = posX + gTexture->getWidth();

    if((pA >= oA && pA <= oB) || (pB >= oA && pB <= oB) ){ //if aligned on x
        if((pPos->y) == posY + height){
            return true;
        }
    }
    return false;
}

int GameObject::onHit(BulletType bulletType){
    return 0;
}

void GameObject::rebirth(){
    dead = false;
}

bool GameObject::isDead(){
    return dead;
}

int GameObject::getWidth(){
    return width;
}

int GameObject::getHeight(){
    return height;
}

LTexture* GameObject::getTexture(){
    return gTexture;
}
