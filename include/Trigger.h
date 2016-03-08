#ifndef TRIGGER_H
#define TRIGGER_H

#include <GameObject.h>
#include <Character.h>

/**
(invisible) game object made for triggering something when hit.
*/
class Trigger : public GameObject
{
    public:
        Trigger(SDL_Renderer* renderer, SDL_Window* window, int pX, int pY, LTextureFactory* lTextureFactory, int triggerId);

        virtual ~Trigger();
        virtual void init();
        virtual void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel);
        virtual void onCollision();

        virtual bool checkCollision(const SDL_Rect& b, bool checkOnly = false);

        virtual int move(PlayerPosition* playerPos);
        virtual std::string getTextureName();

        void setCharacter(Character* character);
    protected:
    private:
        int triggerId;
        bool hit = false;
        bool triggered = false;

        Character* character = NULL;
};

#endif // TRIGGER_H
