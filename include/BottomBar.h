#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H
#include <string>
#include <sstream>
#include <Utils/LTexture.h>
#include <Utils/IRenderedElement.h>
#include <SDL.h>
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <Utils/LTimer.h>
#include <PlayerInventory.h>

//enum HitDamage{HURT, DEATH, POISON};

/**
Represents the bar that lay at the bottom of the screen. Also keeps the player health, points, and whether the level is finished.
*/
class BottomBar : IRenderedElement
{
    public:
        BottomBar(PlayerInventory* inventory);
        virtual ~BottomBar();

        void handleEvent( SDL_Event& e );

        void render(SDL_Renderer* gRenderer, const SDL_Rect &visibleLevel);

        void takeHit(int nb);
        void addPoints(int points);
        void eatBanana();
        void levelCompleted();

        bool isPlayerDead();
        bool isLevelFinished();
        bool areAllBananasEaten();
        void rebirth();
        void setTotalBananas(int totalBananas);
        void setMaxLevelTime(int maxLevelTime);
        void startOrResumeLevelTimer();
        void stopLevelTimer();
    protected:
    private:
        TTF_Font *font;
        SDL_Rect bottomViewport;
        SDL_Rect messageRect;

        PlayerInventory* playerInventory;

        LTimer redTimer, levelTimer;
        int health, points, totalBananas, currentBananas;
        int maxLevelTime; //in seconds
        bool levelFinished;
       /* SDL_Color fontColor;*/
};

#endif // BOTTOMBAR_H
