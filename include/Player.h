#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <cmath>
#include <SDL.h>
#include <Utils/LTexture.h>
#include <Utils/LTimer.h>
#include <Utils/IRenderedElement.h>
#include <Map.h>
#include <BallisticEngine.h>
#include <PlayerInventory.h>
#include <../src/Constants.h>

/**
Contains both model and logic for the player.
*/
class Player : IRenderedElement
{
    public:
        Player(SDL_Renderer* gRenderer, SDL_Window* gWindow, BallisticEngine* ballisticEngine, PlayerInventory* playerInventory);
        virtual ~Player();

		void handleEvent( SDL_Event& e );
		void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel);

		void move(Map* map); //move player according to key events (and gravity)

        void gravity(Map* map); //exert gravity on player

        void initPos();
        void die(); //start death animation
        bool isDying(); //ongoing death animation
        bool hasDied(); //player dead and death animation finished
        bool inPause();
        void setPause(bool pause);
        void reinit(); //reset initial position in new level
        PlayerPosition* getPos();
    protected:
    private:
        SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
        LTexture* gAnimRight;
        LTexture* gAnimLeft;
        LTexture* gJumpTextureRight;
        LTexture* gJumpTextureLeft;
        LTexture* gFireTextureRight;
        LTexture* gFireTextureLeft;
        LTexture* gAnimDeath;

        BallisticEngine* ballisticEngine;

        PlayerPosition pos;
        PlayerInventory* inventory;

		double mVelY, mVelX, lastVelX;

		int playerSpeed, playerSpeedInJump;

		int frame = 0;
		bool died = false;
		bool init = false;
		bool pause = false;

        SDL_Rect mCollider;

        LTimer walkTimer;
        LTimer jumpTimer;
        LTimer fireTimer;
        LTimer dieTimer;
        bool superJump = false;

        bool facingLeft();
};

#endif // PLAYER_H
