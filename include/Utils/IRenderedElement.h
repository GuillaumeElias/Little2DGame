#ifndef IRENDEREDELEMENT_H
#define IRENDEREDELEMENT_H
#include <SDL.h>

static const SDL_Rect NULL_RECT = { 0, 0, 0, 0 };

/**
Interface for all elements that are rendered on the screen.
*/
class IRenderedElement
{
    public:
        //virtual void handleEvent( SDL_Event& e ) = 0;
        virtual void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel) = 0;
    protected:
    private:
};

#endif // IRENDEREDELEMENT_H
