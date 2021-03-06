#include "BottomBar.h"
#include "SoundEngine.h"

SDL_Texture* loadTexture( std::string path, SDL_Renderer* gRenderer );

BottomBar::BottomBar(PlayerInventory* inventory) : playerInventory(inventory){
    levelFinished = false;

    //load font for text
    font = TTF_OpenFont("res/WHITRABT.TTF", 20);
    if (font == NULL){
        printf ("TTF_OpenFont() Failed: " , TTF_GetError());
    }

    //initialize view-port for bottom bar
    bottomViewport.x = 0;
    bottomViewport.y = SCREEN_HEIGHT - MARGIN_BOTTOM + 9;
    bottomViewport.w = SCREEN_WIDTH;
    bottomViewport.h = MARGIN_BOTTOM;

    messageRect.x = 0;
    messageRect.y = 0;
    messageRect.h = 20;

    health = PLAYER_INIT_HEALTH;
    points = 0;
    currentBananas = 0;
    totalBananas = 0;
    pazookVictories = 0;
    maxLevelTime = -1;
    bossLife = -1;
}

BottomBar::~BottomBar()
{
    TTF_CloseFont(font);
    delete font;
}

void BottomBar::takeHit(int nb){
    //starts timer for red blink
    redTimer.start();
    health-=nb;
    SoundEngine::getInstance()->soundEvent(HURT);
}

void BottomBar::addPoints(int points){
    this->points += points;
}

void BottomBar::eatBanana(){
    this->currentBananas++;
    SoundEngine::getInstance()->soundEvent(BANANA);
}

bool BottomBar::isPlayerDead(){
    if(maxLevelTime > 0 && levelTimer.getTicks() / 1000 > maxLevelTime){
        return true;
    }

    return health <= 0;
}

bool BottomBar::isLevelFinished(){
    return levelFinished;
}

bool BottomBar::areAllBananasEaten(){
    return currentBananas >= totalBananas;
}

void BottomBar::rebirth(){
    stopLevelTimer();
    levelFinished = false;
    health = PLAYER_INIT_HEALTH;
    pazookVictories = 0;
    currentBananas = 0;
}

void BottomBar::setTotalBananas(int totalBananas){
    this->totalBananas = totalBananas;
}

void BottomBar::setMaxLevelTime(int maxLvlTime){
    this->maxLevelTime = maxLvlTime;
}

void BottomBar::startOrResumeLevelTimer()
{
    if(maxLevelTime > 0){
        if(!levelTimer.isStarted() || levelTimer.isPaused()){
            levelTimer.start();
        }
    }
}

void BottomBar::stopLevelTimer(){
    levelTimer.stop();
}

void BottomBar::levelCompleted(){
    levelFinished=true;
}

void BottomBar::render(SDL_Renderer* gRenderer, const SDL_Rect &visibleLevel){
    SDL_Color color = {255, 255, 255};

    //if redTimer is started, show text in red
    if(redTimer.isStarted()){
        if(redTimer.getTicks() > BOTTOM_BAR_RED_TIME){
            redTimer.stop();
        }else{
            color = {255, 0, 0};
        }
    }

    //build text
    std::ostringstream messageOss;
    messageOss << "HEALTH: " << health << "   POINTS: " << points << "    ITEMS:";
    if(totalBananas > 0){
        messageOss << "             BANANAS: " << currentBananas << "/" << totalBananas;
    }else if(maxLevelTime > 0){
        int seconds = levelTimer.getTicks() / 1000;
        if(seconds > maxLevelTime){
            messageOss << "             TOO LATE FATTY!";
        }else{
            messageOss << "             DEAD IN: " << (maxLevelTime - seconds);
        }
    }else if(bossLife > 0){
        messageOss << "        BOSS LIFE: " << bossLife << "/" << BOSS_LIFE;
    }
    std::string message = messageOss.str();
    messageOss.str("");
    messageOss.clear();

    //converts text into texture
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

	SDL_FreeSurface(surfaceMessage);

    messageRect.w = 15 * message.length();

    //set draw color and view-port
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderSetViewport( gRenderer, &bottomViewport );

    //Render texture to screen and destroy it
    SDL_RenderCopy( gRenderer, texture, NULL, &messageRect );
    SDL_DestroyTexture( texture );

    //render each item in player inventory
    std::map<int, Item *> items = playerInventory->getItems();
    for (std::map<int, Item *>::iterator it=items.begin(); it!=items.end(); ++it){
        Item * item = it->second;

        item->renderInInventory(ITEM_IN_INVENTORY_MARGIN_LEFT + it->first * ITEM_IN_INVENTORY_WIDTH, ITEM_IN_INVENTORY_MARGIN_TOP/(item->getHeight()/10));
    }
}

void BottomBar::incrementPazookVictory(){
    pazookVictories++;
    addPoints(PAZOOK_WINNING_POINTS);
}

int BottomBar::getPazookVictories(){
    return pazookVictories;
}

void BottomBar::setBossLife(int bossLife){
    this->bossLife = bossLife;
}
