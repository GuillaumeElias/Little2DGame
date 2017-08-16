#ifndef DIALOGPLAYER_H
#define DIALOGPLAYER_H

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <SDL.h>
#include <SDL_ttf.h>
#include <Utils/LTimer.h>
#include <Utils/LTexture.h>
#include <../src/Constants.h>
#include <Utils/IRenderedElement.h>
#include <Character.h>

static const std::string GAME_OVER_TEXT = "Come on...";

/**
Engine for displaying dialogs.
*/
class DialogPlayer : public IRenderedElement{
    public:
        DialogPlayer(SDL_Renderer* renderer, SDL_Window* window);
        virtual void handleEvent( SDL_Event& e );
        virtual void render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel = NULL_RECT);

        void loadLevel(int nb); //loads the text for the end of a level (-1 => intro)
        bool loadCharacterDialog(int levelNb, Character* character); //loads text for character dialog
        void printGameOver(); //shows game over message
        bool isFinished();
        virtual ~DialogPlayer();
        int getNumberOfLines();
    protected:
        SDL_Rect textViewport;
        SDL_Renderer* gRenderer;

        bool allowDisplayNextSentence;
        bool finished;
        int currentLine;
        LTimer sentenceTimer;

        void jumpToNextLine();
    private:
        SDL_Window* gWindow;

        SDL_Color color;
        TTF_Font *font;
        SDL_Rect messageRect;

        Character* character = NULL;

        LTimer letterTimer;
        std::vector<std::string> lines;
        int currentLetter;

        bool loadText(std::string filePath); //load text from file
};

#endif // DIALOGPLAYER_H
