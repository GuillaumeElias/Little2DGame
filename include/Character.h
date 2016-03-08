#ifndef CHARACTER_H
#define CHARACTER_H

#include <GameObject.h>

/**
Neutral character defined by an animation and can be linked to a dialog.
*/
class Character : public GameObject
{
    public:
        Character(SDL_Renderer* renderer, SDL_Window* window, int pX, int pY, LTextureFactory* lTextFact, int characterId);
        virtual ~Character();

        virtual void init();
        virtual void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel);

        void renderInDialog(SDL_Renderer* gRenderer); //renders the character in a dialog
        virtual void onCollision();

        virtual int move(PlayerPosition* playerPos);
        virtual std::string getTextureName();
        int getId();
    protected:
    private:
        int characterId;
        std::stringstream textureName;
        SDL_Rect clip;
        int currentSpriteNb = 0;
        LTimer moveTimer;

};

#endif // CHARACTER_H
