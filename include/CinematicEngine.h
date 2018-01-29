#ifndef CINEMATICENGINE_H
#define CINEMATICENGINE_H

#include <vector>

#include "IRenderedElement.h"
#include "LTexture.h"
#include "LTimer.h"

enum CinematicState {OFF, INTRO, OUTRO};

class CinematicEngine : public IRenderedElement
{
    public:
        CinematicEngine(SDL_Renderer* renderer, SDL_Window* window);
        void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel = NULL_RECT);

        CinematicState getState() const;

        virtual ~CinematicEngine();
    protected:
    private:
        SDL_Renderer* gRenderer;
        SDL_Window* gWindow;
        std::vector<LTexture *> sceneTextures;
        SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
        LTimer sceneTimer, frameTimer;

        CinematicState state = OFF;
        int scene = 0;
        int frame = 0;
        int frameTime = 30;
        int sceneTime = 850;
        int totalScenes = 9;
        bool ending = false;
};

#endif // CINEMATICENGINE_H
