#ifndef PAZOOKENGINE_H
#define PAZOOKENGINE_H

#include <ctime>
#include <SDL.h>
#include <DialogPlayer.h>
#include <Utils/LTexture.h>
#include <Character.h>
#include <LTextureFactory.h>

class PazookEngine : public DialogPlayer
{
    public:
        PazookEngine(SDL_Renderer* renderer, SDL_Window* window, LTextureFactory * textureFact);
        virtual ~PazookEngine();

        void handleEvent( SDL_Event& e ) override;
        void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel = NULL_RECT) override;
        bool loadPazookGame(int levelNb, Character* linkedCharacter);
        bool isInSelection();
        bool hasWon();
        void reset();
    protected:
    private:
        LTextureFactory * textureFactory;
        std::vector<int> availableDeck;
        LTexture* cardTexture1;
        LTexture* cardTexture2;
        LTexture* cardTexture3;
        LTexture* cardSelectionTexture;
        LTexture* pazookTexture;
        LTexture* hoozahTexture;
        LTexture* answerSelectionTexture;
        int selection;
        bool finished = false;
        bool inSelection = false;
        bool won = false;

        LTexture* pickAndBuildCard();
        void onEnterPressed();
        int getSelectionMax();
        bool inCardPicking();
        bool inPazookHozaah();
};

#endif // PAZOOKENGINE_H
