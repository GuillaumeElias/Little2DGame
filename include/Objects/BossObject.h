#ifndef BOSSOBJECT_H
#define BOSSOBJECT_H

#include "GameObject.h"
#include "BottomBar.h"

class BallisticEngine;
class Map;

enum BossAttackStep{ IDLE, DESCENT, ATTACK, ASCENT, VULNERABLE };

class BossObject : public GameObject
{
    public:
        BossObject(SDL_Renderer* gRenderer, SDL_Window* gWindow, BottomBar* bottomB, int posX, int posY, LTextureFactory* lTextFact, BallisticEngine * ballisticEng, PlayerPosition * playerPos, Map * map );

        virtual ~BossObject();

        void init();
        void rebirth() override;

        int onHit(BulletType bulletType); //returns number of points yielded by the hit
        void onCollision();

        void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel);

        int move(PlayerPosition* playerPos);

        std::string getTextureName();

    protected:
    private:

        bool isPlayerAtRight();
        void activateEnd();

        int initPosX, initPosY;
        short curFrame = 0;

        LTimer hitTimer, animTimer, idleTimer, attackTimer, fireTimer, vulnerableTimer;
        int life;
        Direction direction;

        SDL_Rect collider;

        BallisticEngine * ballisticEngine;
        PlayerPosition * playerPosition;
        BottomBar * bottomBar;
        Map * levelMap;

        LTexture * normalTexture;
        LTexture * vulnerableTexture;

        BossAttackStep bossAttackStep;
};

#endif // BOSSOBJECT_H
