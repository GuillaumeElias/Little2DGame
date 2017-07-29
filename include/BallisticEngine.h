#ifndef BALLISTICENGINE_H
#define BALLISTICENGINE_H

#include <Utils/IRenderedElement.h>
#include <Map.h>

struct BulletPosition : Position{
    Direction direction;
    bool operator<(const BulletPosition &other) const { return x < other.x; }
    bool operator==(const BulletPosition &other) const { return x == other.x && y == other.y; }
};

/**
Handles all bullets in the game.
*/
class BallisticEngine : public IRenderedElement
{
    public:
        BallisticEngine(SDL_Renderer* gRenderer);

        void fireBullet(const Position position, Direction direction);

        void move(Map* map);
        void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel = NULL_RECT);


        virtual ~BallisticEngine();
    protected:
    private:
        std::vector<BulletPosition> bullets;
        LTexture* bulletTexture;
};

#endif // BALLISTICENGINE_H
