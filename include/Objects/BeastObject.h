#ifndef BEASTOBJECT_H
#define BEASTOBJECT_H

#include <GameObject.h>
#include <BottomBar.h>

class Map;

/**
Beast-like monster. Runs back and forth between collisions.
*/
class BeastObject : public GameObject
{
    public:
        BeastObject(SDL_Renderer* gRenderer, SDL_Window* gWindow, BottomBar* bottomBar, int posX, int posY, LTextureFactory* lTextFact,
                    Map* map, bool (Map::*checkCol) (const SDL_Rect &, bool) );
        virtual ~BeastObject();

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

        int initPosX, initPosY;
        short curFrame = 0;

        LTimer hitTimer;
        LTimer animTimer;
        int life;
        Direction direction;

        SDL_Rect collider;
        SDL_Rect _playerRect;

        Map* map;
        bool (Map::*checkCollision) (const SDL_Rect &, bool);
};

#endif // BEASTOBJECT_H
