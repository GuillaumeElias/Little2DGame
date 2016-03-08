#ifndef ZOMBIEOBJECT_H
#define ZOMBIEOBJECT_H

#include <GameObject.h>
#include <BottomBar.h>

/**
Zombie monster. Goes back and forth within set distance.
*/
class ZombieObject : public GameObject
{
    public:
        virtual ~ZombieObject();

        ZombieObject(SDL_Renderer* gRenderer, SDL_Window* gWindow, BottomBar* bottomBar, int posX, int posY, LTextureFactory* lTextFact, int distance);
        void init();

        int onHit(BulletType bulletType); //returns number of points yielded by the hit
        void onCollision();

        void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel);

        int move(PlayerPosition* playerPos);
        void rebirth();

        std::string getTextureName();

    protected:
    private:
        BottomBar* bottomBar;
        int initPosX;
        LTimer hitTimer;
        int life;
        int distance;
        Direction direction;

        SDL_Rect clip;
};

#endif // ZOMBIEOBJECT_H
