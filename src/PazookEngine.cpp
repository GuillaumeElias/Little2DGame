#include "PazookEngine.h"

PazookEngine::PazookEngine(SDL_Renderer* renderer, SDL_Window* window, LTextureFactory * textureFact) :
    DialogPlayer(renderer, window),
    textureFactory(textureFact),
    selection(1)
{
    textViewport.y = PAZOOK_DIALOG_TOP_MARGIN;

    reset();

    cardSelectionTexture = new LTexture(gRenderer, false);
    if( !cardSelectionTexture->loadFromFile("img/pazook/cardSelection.png", { 0xFF, 0xFF, 0xFF } ) ){
        printf( "Failed to load pazook game card selection texture!\n" );
        return;
    }

    pazookTexture = new LTexture(gRenderer, false);
    if( !pazookTexture->loadFromFile("img/pazook/pazook.png", { 0xFF, 0xFF, 0xFF } ) ){
        printf( "Failed to load pazook game pazook texture!\n" );
        return;
    }
    hoozahTexture = new LTexture(gRenderer, false);
    if( !hoozahTexture->loadFromFile("img/pazook/hoozah.png", { 0xFF, 0xFF, 0xFF } ) ){
        printf( "Failed to load pazook game hoozah texture!\n" );
        return;
    }
    answerSelectionTexture = new LTexture(gRenderer, false);
    if( !answerSelectionTexture->loadFromFile("img/pazook/answerSelection.png", { 0xFF, 0xFF, 0xFF } ) ){
        printf( "Failed to load pazook game answer selection texture!\n" );
        return;
    }
}

PazookEngine::~PazookEngine()
{
    delete cardSelectionTexture;
    delete pazookTexture;
    delete hoozahTexture;
    delete answerSelectionTexture;
}

void PazookEngine::handleEvent(SDL_Event& e){
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        switch( e.key.keysym.sym ){
            case SDLK_RIGHT:
                if(selection < getSelectionMax() ){
                    selection++;
                }
            break;
            case SDLK_LEFT:
                if(selection > 0){
                    selection--;
                }
            break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                onEnterPressed();
            break;
        }
    }
}

void PazookEngine::onEnterPressed(){
    if(inCardPicking()){
        allowDisplayNextSentence = true;
        inSelection = false;
        jumpToNextLine();
        selection = 0;
    }else if(inPazookHozaah()){
        if(availableDeck.back() % 2 == selection){ //if parity of card stayed in deck == pazook->even
            won = true; //PLAYER WON
        }else{
            currentLine = getNumberOfLines() - 3; //PLAYER LOST -> skip congratulation message
        }
        allowDisplayNextSentence = true;
        inSelection = false;
        jumpToNextLine();
    }
}

LTexture* PazookEngine::pickAndBuildCard(){
    std::stringstream texturePath;
    texturePath << "img/pazook/" << availableDeck.back() << ".png";
    LTexture * texture = textureFactory->fetchTexture(texturePath.str(), false);
    texturePath.clear();
    availableDeck.pop_back();

    return texture;
}

bool PazookEngine::loadPazookGame(int levelNb, Character* linkedCharacter){
    return DialogPlayer::loadCharacterDialog(levelNb, linkedCharacter);
}

bool PazookEngine::isInSelection(){
    return inSelection;
}

void PazookEngine::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){
    if(inCardPicking() || inPazookHozaah()){
        allowDisplayNextSentence = false;
        inSelection = true;
    }else if(won && currentLine == getNumberOfLines() - 2){
        currentLine = 100; //forces the dialog to be terminated (thus skipping game over message)
    }

    DialogPlayer::render(gRenderer, mapVisibleLevel);

    if(inCardPicking() && sentenceTimer.getTicks() > DIALOG_TIME_SENTENCE){
        cardTexture1->render(PAZOOK_CARD1_X, PAZOOK_CARDS_MARGIN_TOP);
        cardTexture2->render(PAZOOK_CARD2_X, PAZOOK_CARDS_MARGIN_TOP);
        cardTexture3->render(PAZOOK_CARD3_X, PAZOOK_CARDS_MARGIN_TOP);

        //renders selection texture where it should be
        switch(selection){
            case 0 : cardSelectionTexture->render(PAZOOK_CARD1_X, PAZOOK_CARDS_MARGIN_TOP);
            break;
            case 1 : cardSelectionTexture->render(PAZOOK_CARD2_X, PAZOOK_CARDS_MARGIN_TOP);
            break;
            case 2 : cardSelectionTexture->render(PAZOOK_CARD3_X, PAZOOK_CARDS_MARGIN_TOP);
            break;
        }
    }else if(inPazookHozaah() && sentenceTimer.getTicks() > DIALOG_TIME_SENTENCE){
        pazookTexture->render(PAZOOK_CARD1_X, PAZOOK_CARDS_MARGIN_TOP);
        hoozahTexture->render(PAZOOK_CARD3_X, PAZOOK_CARDS_MARGIN_TOP);

        switch(selection){
            case 0 : answerSelectionTexture->render(PAZOOK_CARD1_X, PAZOOK_CARDS_MARGIN_TOP);
            break;
            case 1 : answerSelectionTexture->render(PAZOOK_CARD3_X, PAZOOK_CARDS_MARGIN_TOP);
            break;
        }
    }
}

int PazookEngine::getSelectionMax(){
    if(inCardPicking()){
        return PAZOOK_NUMBER_MAX - 1;
    }else if(inPazookHozaah()){
        return 1;
    }
    return 0;
}

bool PazookEngine::hasWon(){
    return won;
}

void PazookEngine::reset(){
    currentLine = 0;
    selection = 1;
    won = false;
    finished = false;

    availableDeck = {1,2,3,4,5};
    srand(unsigned(time(NULL)));
    std::random_shuffle(availableDeck.begin(), availableDeck.end());

    //load textures
    cardTexture1 = pickAndBuildCard();
    cardTexture2 = pickAndBuildCard();
    cardTexture3 = pickAndBuildCard();
}

bool PazookEngine::inCardPicking(){
    return currentLine == getNumberOfLines() - 7;
}

bool PazookEngine::inPazookHozaah(){
    return currentLine == getNumberOfLines() - 5;
}
