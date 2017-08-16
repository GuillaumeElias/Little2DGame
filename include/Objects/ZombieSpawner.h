#ifndef ZOMBIESPAWNER_H
#define ZOMBIESPAWNER_H

#include "GameObject.h"
#include <BottomBar.h>

class Map;

class ZombieSpawner : public GameObject
{
    public:
        ZombieSpawner(SDL_Renderer* gRenderer, SDL_Window* gWindow, BottomBar* bottomB, int posX, int posY, LTextureFactory* lTextFact, Map* map, int direction );
        virtual ~ZombieSpawner();

        void init();

        int move(PlayerPosition* playerPos);
        void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel);

        std::string getTextureName();
        void onCollision();

    protected:
    private:

        void spawnZombie();

        int initPosX, initPosY;
        short curFrame = 0;

        LTimer animTimer;
        LTimer spawnTimer;

        Direction direction;
        SDL_Rect clip;
        SDL_Rect collider;

        BottomBar * bottomBar;
        Map* map;
};

#endif // ZOMBIESPAWNER_H
