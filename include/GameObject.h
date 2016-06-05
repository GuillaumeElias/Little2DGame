#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <fstream>
#include <sstream>
#include <SDL.h>
#include <Utils/LTimer.h>
#include <Utils/IRenderedElement.h>
#include <Utils/LTexture.h>
#include <Utils/LTextureFactory.h>

/**
Interface for all game objects (monsters, items, elevators and triggers)
*/
class GameObject : public IRenderedElement
{
    public:
        GameObject(SDL_Renderer* gRenderer, SDL_Window* gWindow, int posX, int posY, LTextureFactory* lTextureFactory);
        virtual ~GameObject();
        virtual void init(); //has to be called for each object

        virtual void handleEvent( SDL_Event& e ); //if object needs to react to key events
        virtual void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel);

        virtual bool checkCollision(const SDL_Rect &collider, bool checkOnly); //returns true if object is hit
        virtual int onHit(BulletType bulletType); //returns number of points yielded by the hit

        virtual void onCollision() = 0;
        virtual int move(PlayerPosition* playerPos) = 0; //returns 0 or trigger id if one has been hit

        virtual void rebirth(); //has to be called when level is restarted

        virtual std::string getTextureName() = 0;

        LTexture* getTexture();
        bool isDead();
        bool isDisabled();
        void setDisabled(bool disabled);
        int getWidth();
        int getHeight();
    protected:
        SDL_Renderer* gRenderer;
        SDL_Window* gWindow;
        LTextureFactory* lTextureFactory;

        LTexture* gTexture = NULL;
        int posX, posY;
        int width, height;
        bool dead = false;
        bool disabled = false;

        bool isPlayerAbove(PlayerPosition* playerPos);
        bool isPlayerBelow(PlayerPosition* playerPos);
};

#endif // GAMEOBJECT_H
