#ifndef ENDOBJECT_H
#define ENDOBJECT_H

#include <GameObject.h>
#include <BottomBar.h>

/**
Ends the current level when hit.
*/
class EndObject : public GameObject
{
    public:
        EndObject(SDL_Renderer* gRenderer, SDL_Window* gWindow, BottomBar* bottomBar, int posX, int posY, LTextureFactory* lTextFact);
        virtual ~EndObject();
        void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel);
        void onCollision();
        int move(PlayerPosition* playerPos);
        std::string getTextureName();
    protected:
    private:
        BottomBar* bottomBar;
        SDL_Rect gSpriteClips[ END_OBJECT_ANIMATION_FRAMES ];
        LTexture* gAnim;

        LTimer animTimer;
        int frame = 0;
};

#endif // ENDOBJECT_H
