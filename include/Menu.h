#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <Utils/IRenderedElement.h>
#include <Utils/LTexture.h>

static const int NUMBER_MENU_ITEMS = 2;
enum MenuChoice {NONE, PLAY, LEAVE};

/**
Pre-game menu.
*/
class Menu : public IRenderedElement
{
    public:
        Menu(SDL_Renderer* renderer, SDL_Window* window);
        void handleEvent( SDL_Event& e );
        void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel = NULL_RECT);
        virtual ~Menu();
        MenuChoice getChoice();
    protected:
    private:
        LTexture* backgroundTexture;
        LTexture* selectionTexture;
        SDL_Renderer* gRenderer;
        SDL_Window* gWindow;

        int selection = 0;
        MenuChoice selectedChoice;
};

#endif // MENU_H
