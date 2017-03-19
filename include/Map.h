#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <SDL.h>
#include <BottomBar.h>
#include <GameObject.h>
#include <Character.h>
#include <Trigger.h>
#include <PlayerInventory.h>
#include <Utils/IRenderedElement.h>
#include <Utils/LMapTexture.h>
#include <Utils/LTextureFactory.h>
#include <Objects/BlobObject.h>
#include <Objects/ElevatorObject.h>
#include <Objects/EndObject.h>
#include <Objects/ZombieObject.h>
#include <Objects/BeastObject.h>
#include <Objects/BananaObject.h>

/**
Contains the terrain and all the game objects for each level. Same instance throughout the whole game.
*/
class Map : IRenderedElement
{
    public:
        Map(SDL_Renderer* gRenderer, SDL_Window* gWindow, BottomBar* bottomBar, LTextureFactory* lTextureFactory, PlayerInventory* playerInventory);
        virtual ~Map();

        void loadLevel(int levelNb); //loads a level
        void unloadLevel(); //this has to be called before loading a new level
        void resetLevel(); //this needs to be called when player has died

        void handleEvent( SDL_Event& e );

		void move(PlayerPosition* playerPos);

		void render(SDL_Renderer* gRenderer, const SDL_Rect &visibleLevel);

		SDL_Rect getVisibleLevel();

		bool isLevelFinished();

		int getLevelWidth();
		std::vector<GameObject*>* getGameObjects();
		BottomBar* const getBottomBar();

        bool checkCollision(const SDL_Rect &collider, bool terrainOnly); //(terrainOnly means ignore the objects)

        Character* getCharacter(int characterId);
    protected:
    private:
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;
        LTextureFactory* lTextureFactory;
        BottomBar* bottomBar;
        PlayerInventory* playerInventory;

        LMapTexture* gLevelTexture;
        SDL_Rect visibleLevel;

        std::vector<GameObject*> gameObjects;
        std::map <int, Character*> characters; //keeping a map of the characters (also present in gameObjects)

        Uint32 getPixel(Uint32* pixels, int x, int y); //get pixel or the level for collision check
};

#endif // MAP_H
