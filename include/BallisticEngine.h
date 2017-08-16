#ifndef BALLISTICENGINE_H
#define BALLISTICENGINE_H

#include "Utils/IRenderedElement.h"
#include "Map.h"
#include "BottomBar.h"

struct BulletPosition : Position{
    Direction direction;
    bool enemy = false;
    bool operator<(const BulletPosition &other) const { return x < other.x; }
    bool operator==(const BulletPosition &other) const { return x == other.x && y == other.y; }
};

/**
Handles all bullets in the game.
*/
class BallisticEngine : public IRenderedElement
{
    public:
        BallisticEngine(SDL_Renderer* gRenderer, BottomBar * bottomBa);

        void fireBullet(const Position position, Direction direction, bool enemy = false);
        void clearBullets();

        void move(Map* map);
        void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel = NULL_RECT);
        void setPlayerPosition(PlayerPosition * playerPosition);

        virtual ~BallisticEngine();
    protected:
    private:
        PlayerPosition * playerPosition;
        BottomBar * bottomBar;
        std::vector<BulletPosition> bullets;
        LTexture* playerBulletTexture;
        LTexture* enemyBulletTexture;
};

#endif // BALLISTICENGINE_H
