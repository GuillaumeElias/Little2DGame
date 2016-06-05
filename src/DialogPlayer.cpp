#include "DialogPlayer.h"

DialogPlayer::DialogPlayer(SDL_Renderer* renderer, SDL_Window* window) : gRenderer(renderer), gWindow(window){

    //load font for message
    font = TTF_OpenFont("res/WHITRABT.TTF", 20);
    if (font == NULL){
        printf ("TTF_OpenFont() Failed: " , TTF_GetError());
    }

    color = {0, 0, 0}; //background color

    messageRect.x = 0;
    messageRect.y = 0;
    messageRect.h = 20;

    textViewport.y = SCREEN_HEIGHT / 3;
    textViewport.w = SCREEN_WIDTH ;
    textViewport.h = SCREEN_HEIGHT - SCREEN_HEIGHT / 3;
}

DialogPlayer::~DialogPlayer(){
    //dtor
}

void DialogPlayer::loadLevel(int nb){
    //get text file for level dialog
    std::ostringstream levelPathBaseOss;
    if(nb == -1){ // -1 means intro
        levelPathBaseOss << "text/intro";
    }else{
        levelPathBaseOss << "text/level" << nb;
    }
    levelPathBaseOss.clear();

    loadText(std::string(levelPathBaseOss.str() + ".txt"));
}

bool DialogPlayer::loadCharacterDialog(int levelNb, Character* linkedCharacter){
    character = linkedCharacter;

    //get text file for character dialog
    std::ostringstream levelPathBaseOss;
    levelPathBaseOss << "text/level" << levelNb << "_" << character->getId() << ".txt";
    levelPathBaseOss.clear();

    return loadText(levelPathBaseOss.str());
}

bool DialogPlayer::loadText(std::string filePath){
    //clear previous text
    lines.clear();

    //load text from file
    std::cout << "DialogPlayer::loadText " << filePath << std::endl;

    std::ifstream txtFile(filePath.c_str());
    std::string line;
    while (std::getline(txtFile, line)){
        lines.push_back(line);
    }

    currentLine = 0;
    currentLetter = 0;
    finished = false;

    return lines.size() > 0;
}

void DialogPlayer::printGameOver(){
    lines.clear();
    lines.push_back(GAME_OVER_TEXT);

    currentLine = 0;
    currentLetter = 0;
    finished = false;
}


void DialogPlayer::handleEvent(SDL_Event& e){
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        switch( e.key.keysym.sym ){
            case SDLK_KP_ENTER:
                //TODO
            break;
        }
    }
}


void DialogPlayer::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){

    //if reached the end of dialog
    if(currentLine >= lines.size()){
        finished = true;
        if(character != NULL){
            character->onDialogEnd();
            character = NULL;
        }
        return;
    }

    //starts letter timer
    if(!letterTimer.isStarted()){
        SDL_RenderClear( gRenderer );
        letterTimer.start();
    }

    std::string line = lines.at(currentLine);

    if(sentenceTimer.isStarted()){ //if at the sentence out waiting timer
        if(sentenceTimer.getTicks() > DIALOG_TIME_SENTENCE){
            sentenceTimer.stop();
            currentLetter = 0;
            currentLine++;          //GO TO NEXT LINE
            return;
        }
    }else if(currentLetter >= line.size()){ //if last letter displayed
        sentenceTimer.start(); //starts sentence out waiting timer
    }else if(letterTimer.getTicks() > DIALOG_TIME_LETTER){
        letterTimer.stop();
        currentLetter++;          //GO TO NEXT LETTER
        return;
    }

    //build message based on current number of displayed letters
    std::string message = line.substr(0, currentLetter);

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

	SDL_FreeSurface(surfaceMessage);

    if(character != NULL){ //if character dialog
        textViewport.x = DIALOG_LEFT_MARGIN + character->getWidth() + DIALOG_CHARACTER_RIGHT_MARGIN;
    }else{ //if level dialog
        textViewport.x = DIALOG_LEFT_MARGIN;
    }

    messageRect.w = 16 * message.length();
    size_t n = std::count(message.begin(), message.end(), ' ');
    messageRect.w -= n;

    //set draw color, clear buffer and render
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderSetViewport( gRenderer, &textViewport );
    SDL_RenderClear( gRenderer );
    SDL_RenderCopy( gRenderer, texture, NULL, &messageRect );
    SDL_DestroyTexture( texture ); //destroy created texture

    if(character != NULL){
        character->renderInDialog(gRenderer); //displays the character in the dialog
    }
}

bool DialogPlayer::isFinished(){
    return finished;
}
