#ifndef BANANAOBJECT_H
#define BANANAOBJECT_H

#include <vector>
#include <GameObject.h>
#include <BottomBar.h>

/**
Ends the current level when hit.
*/
class BananaObject : public GameObject
{
    public:
        BananaObject(SDL_Renderer* gRenderer, SDL_Window* gWindow, BottomBar* bottomBar, int posX, int posY, LTextureFactory* lTextFact, const std::vector<GameObject*>* gameObjects);
        virtual ~BananaObject();
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

        const std::vector<GameObject*>* gameObjects;
};

#endif // BANANAOBJECT_H
