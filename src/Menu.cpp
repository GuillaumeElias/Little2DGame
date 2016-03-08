#include "Menu.h"

Menu::Menu(SDL_Renderer* renderer, SDL_Window* window) : gRenderer(renderer), gWindow(window){
    //load textures
    backgroundTexture = new LTexture(gRenderer, false);
    if( !backgroundTexture->loadFromFile("img/menu.png", {0, 0, 0} ) ){
        printf( "Failed to load menu texture!\n" );
        return;
    }
    selectionTexture = new LTexture(gRenderer, false);
    if( !selectionTexture->loadFromFile("img/selection.png", { 0xFF, 0xFF, 0xFF } ) ){
        printf( "Failed to load menu selection texture!\n" );
        return;
    }
}

void Menu::handleEvent(SDL_Event& e){
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        switch( e.key.keysym.sym ){
            case SDLK_DOWN:
                if(selection < NUMBER_MENU_ITEMS - 1 ){
                    selection++;
                }
            break;
            case SDLK_UP:
                if(selection > 0){
                    selection--;
                }
            break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                if(selection == 0){
                    selectedChoice = PLAY;
                }else if(selection == 1){
                    selectedChoice = LEAVE;
                }
            break;
        }
    }
}

void Menu::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    //renders background
    backgroundTexture->render(0, 0);

    //renders selection texture where it should be
    switch(selection){
        case 0 : selectionTexture->render(290, 180);
        break;
        case 1 : selectionTexture->render(290, 318);
        break;
    }

}

MenuChoice Menu::getChoice(){
    return selectedChoice;
}

Menu::~Menu(){
    //dtor
}
