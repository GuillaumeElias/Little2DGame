#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <../src/Constants.h>
#include <Utils/IRenderedElement.h>
#include <Utils/LTextureFactory.h>
#include <SoundEngine.h>
#include <Player.h>
#include <BottomBar.h>
#include <Map.h>
#include <Menu.h>
#include <DialogPlayer.h>
#include <BallisticEngine.h>
#include <PazookEngine.h>
#include <PlayerInventory.h>

enum GameState { MENU, DIALOG, PAZOOK, PLAYING, GAME_OVER };

/**
Contains game core loop and functions.
*/
class Game{
    public:
        SDL_Window* gWindow = NULL;
        SDL_Renderer* gRenderer = NULL;

        void run();
        bool init();
        void close();

        Game();
        virtual ~Game();
    protected:
    private:
        SDL_Rect topViewport; //game view-port
        Player* player;
        BottomBar* bottomBar;

        LTimer frameTimer;

        GameState gameState;
        int titleNb;

        int level; //current level
        bool listenKeys; //whether key events are listened to
        bool rebirth = false; //flag set to true during rebirth of player
};

#endif // GAME_H
