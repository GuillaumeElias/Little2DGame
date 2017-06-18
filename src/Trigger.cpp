#include "Trigger.h"



Trigger::~Trigger(){

}


Trigger::Trigger(SDL_Renderer* renderer, SDL_Window* window, int pX, int pY, LTextureFactory* lTextFact, int triggerId)
 : GameObject(renderer, window, pX, pY, lTextFact), triggerId(triggerId){

}

void Trigger::init(){
    width = TRIGGER_WIDTH; //set width (always the same)
    height = TRIGGER_DEFAULT_HEIGHT; //set default height (will be reset if a character is linked)
}

void Trigger::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
}

std::string Trigger::getTextureName(){
    return ""; //invisible by nature
}

int Trigger::move(PlayerPosition* playerPos){
    if(hit){
        hit = false;
        triggered = true;
        return triggerId; //return trigger id to whatever hit it
    }else{
        return 0;
    }
}

void Trigger::onCollision(){
    std::cout << "Character trigger hit" << std::endl;
    hit = true; //set hit flag
}

bool Trigger::checkCollision(const SDL_Rect& b, bool checkOnly){
    if(!checkOnly && !triggered && !hit){
        GameObject::checkCollision(b, false); //check only if active
    }

    return false;
}

void Trigger::setCharacter(Character* character){
    this->character = character;
    height = character->getHeight(); //set same height as character's
}

void Trigger::reenable(){
    triggered = false;
    hit = false;
}

int Trigger::getTriggerId(){
    return triggerId;
}
