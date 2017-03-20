#ifndef BLOBOBJECT_H
#define BLOBOBJECT_H

#include <iostream>
#include <Utils/LTimer.h>
#include <GameObject.h>
#include <BottomBar.h>

/**
Blob-looking monster. Jumps up and down.
*/
class BlobObject : public GameObject
{
    public:
        BlobObject(SDL_Renderer* gRenderer, SDL_Window* gWindow, BottomBar* bottomBar, int posX, int posY, LTextureFactory* lTextFact);
        virtual ~BlobObject();

        int onHit(BulletType bulletType); //returns number of points yielded by the hit
        void onCollision();
        int move(PlayerPosition* playerPos);
        std::string getTextureName();
        void rebirth() override;
    protected:
    private:
        BottomBar* bottomBar;

        int initPosY;
        bool up;
        LTimer hitTimer;
        int life;
};

#endif // BLOBOBJECT_H
