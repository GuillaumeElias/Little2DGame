#include "Game.h"


Game::Game(){

    //initialize game view-port
    topViewport.x = 0;
    topViewport.y = 0;
    topViewport.w = SCREEN_WIDTH;
    topViewport.h = SCREEN_HEIGHT - MARGIN_BOTTOM;

    level = -2;
    listenKeys = true;

    //initialize game state
    gameState = MENU;
}

Game::~Game()
{
    //dtor
}


void Game::run(){
	if( !init() ){
		printf( "Failed to initialize!\n" );
	}
	else{
        bool quit = false;
        bool intro = true;
        bool postIntro = false;
        int triggerId = 0;

        SDL_Event e;

        //initialize game engines
        Menu* menu = new Menu(gRenderer, gWindow);
        DialogPlayer* dialogPlayer = new DialogPlayer(gRenderer, gWindow);
        LTextureFactory* lTextureFactory = new LTextureFactory(gRenderer);
        SoundEngine* soundEngine = new SoundEngine();

        PlayerInventory* playerInventory = new PlayerInventory();
        BottomBar* bottomBar = new BottomBar(playerInventory);
        BallisticEngine* ballisticEngine = new BallisticEngine(gRenderer, bottomBar);
        Map* map = new Map(gRenderer, gWindow, bottomBar, lTextureFactory, playerInventory, ballisticEngine);
        CinematicEngine * cinematicEngine = new CinematicEngine(gRenderer, gWindow);
        PazookEngine* pazookEngine = new PazookEngine(gRenderer, gWindow, lTextureFactory);
        Player* player = new Player(gRenderer, gWindow, ballisticEngine, playerInventory);
        ballisticEngine->setPlayerPosition(player->getPos());
        map->setPlayerPosition(player->getPos());

        //load first level
        map->loadLevel(0);

        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );

        frameTimer.start();

        while( !quit ){
            switch(gameState){

                /***************MENU******************/
                case MENU:
                    while( SDL_PollEvent( &e ) != 0 ){
                        if( e.type == SDL_QUIT ){
                            quit = true;
                        }
                        menu->handleEvent( e );
                    }

                    //render menu
                    SDL_RenderClear( gRenderer );
                    menu->render(gRenderer);
                    SDL_RenderPresent( gRenderer );

                    if(menu->getChoice() == PLAY){ //user selected "PLAY"
                        if(level < 0){
                            dialogPlayer->loadLevel(level++); // load intro
                            gameState = DIALOG;
                        }else{
                            menu->setMode(START);
                            gameState = PLAYING;
                            player->setPause(false);
                            soundEngine->resumeMusic();
                        }
                        menu->clearChoice();

                        break;
                    }else if(menu->getChoice() == LEAVE){ //player selected "LEAVE"
                        quit = true;
                        break;
                    }
                break;

                /***************CINEMATIC******************/
                case CINEMATIC:
                    while(SDL_PollEvent( &e ) != 0 ){
                        if( e.type == SDL_QUIT ){
                            quit = true;
                        }else if(listenKeys){
                            dialogPlayer->handleEvent( e );
                        }
                    }

                    if(cinematicEngine->getState() == OFF){
                        gameState = DIALOG;
                        dialogPlayer->loadLevel(level++); // load intro end text
                        soundEngine->stopMusic();
                        intro = false;
                        postIntro = true;
                    }else{
                        SDL_RenderClear( gRenderer );
                        cinematicEngine->render(gRenderer, map->getVisibleLevel());
                        SDL_RenderPresent( gRenderer );
                    }

                break;

                /***************DIALOG******************/
                case DIALOG:
                    while(SDL_PollEvent( &e ) != 0 ){
                        if( e.type == SDL_QUIT ){
                            quit = true;
                        }else if(listenKeys){
                            dialogPlayer->handleEvent( e );
                        }
                    }

                    //set draw color and render dialog
                    dialogPlayer->render(gRenderer);
                    SDL_RenderPresent( gRenderer );

                    if(dialogPlayer->isFinished()){ //if dialog is finished
                        if(level < NB_LEVELS){
                            if(intro){
                                gameState = CINEMATIC;
                                soundEngine->startMusic(-1);
                            }else{
                                gameState = PLAYING;

                                if(postIntro){
                                    soundEngine->startMusic(0);
                                    postIntro = false;
                                }
                            }
                            listenKeys = true;
                            bottomBar->startOrResumeLevelTimer();
                        }else{
                            gameState = MENU; //if game is finished
                            level = -1;
                            map->loadLevel(0);
                            SDL_RenderSetViewport( gRenderer, NULL );
                        }

                        break;
                    }

                break;

                /***************PLAYING******************/
                case PLAYING:
                {
                    while( SDL_PollEvent( &e ) != 0 ){
                        if( e.type == SDL_QUIT ){
                            quit = true;
                        }

                        if(listenKeys){
                             player->handleEvent( e );
                        }
                    }

                    //move player
                    player->move(map);
                    PlayerPosition* playerPos = player->getPos();

                    //move bullets
                    ballisticEngine->move(map);

                    //move map (according to player x)
                    map->move(playerPos);

                    //move all objects
                    std::vector<GameObject*>* gameObjects = map->getGameObjects();
                    for(int i=0; i<gameObjects->size(); i++){
                        if((triggerId = gameObjects->at(i)->move(playerPos)) > 0){ //if a trigger was hit

                            if(triggerId > TRIGGER_DIALOG_PARAM_MASK && triggerId < TRIGGER_PAZOOK_PARAM_MASK){ //if the trigger linked to dialog
                                listenKeys = false;
                                Character* character = map->getCharacter(triggerId - TRIGGER_DIALOG_PARAM_MASK); //fetch character

                                if(character != NULL && dialogPlayer->loadCharacterDialog(level, character)){
                                     gameState = DIALOG;
                                }
                            }else if(triggerId > TRIGGER_PAZOOK_PARAM_MASK){ //if trigger linked to pazook game
                                listenKeys = false;
                                Character* character = map->getCharacter(triggerId - TRIGGER_PAZOOK_PARAM_MASK); //fetch character

                                if(character != NULL && pazookEngine->loadPazookGame(level, character)){
                                    gameState = PAZOOK;
                                    std::vector<GameObject*>* gameObjects = map->getGameObjects();

                                    for(int j=0; j<gameObjects->size(); j++){ //re-enable other pazook triggers
                                        if(Trigger * trigger = dynamic_cast<Trigger *> (gameObjects->at(j))){
                                            if(trigger->getTriggerId() > TRIGGER_PAZOOK_PARAM_MASK && trigger->getTriggerId()!=triggerId){
                                                trigger->reenable();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    //if player is dead
                    if(bottomBar->isPlayerDead()){
                        listenKeys = false;

                        if(player->isDying()){ //in death animation
                            if(player->hasDied()){ //death animation finished
                                player->reinit();
                                map->resetLevel();
                                ballisticEngine->clearBullets();
                                bottomBar->rebirth();
                                dialogPlayer->printGameOver();
                                gameState = DIALOG;
                                SDL_FlushEvent(SDL_KEYDOWN);
                                SDL_FlushEvent(SDL_KEYUP);
                                continue;
                            }
                        }else{
                            listenKeys = false;
                            player->die(); //kill player
                            soundEngine->soundEvent(PLAYER_DEATH);
                        }

                    }else if(bottomBar->isLevelFinished()){ //if level finished
                        listenKeys = false;
                        bottomBar->stopLevelTimer();
                        player->reinit(); //reset player position
                        ballisticEngine->clearBullets();
                        bottomBar->rebirth(); //reset bottom bar
                        map->unloadLevel();

                        dialogPlayer->loadLevel(level); //load end level dialog
                        ++level;
                        soundEngine->startMusic(level);

                        if(level < NB_LEVELS){ // if game not finished
                            map->loadLevel(level); //load next level
                        }

                        gameState = DIALOG; //do to dialog mode

                        continue;
                    }else if(player->inPause()){
                        if(menu->getMode() != PAUSE){
                            soundEngine->pauseMusic();
                            menu->setMode(PAUSE);
                            gameState = MENU;
                            SDL_RenderSetViewport( gRenderer, NULL );
                            continue;
                        }
                    }

                    SDL_RenderClear( gRenderer );
                    SDL_RenderSetViewport( gRenderer, &topViewport );

                    //Render all elements
                    bottomBar->render(gRenderer, map->getVisibleLevel());
                    map->render(gRenderer, map->getVisibleLevel());
                    player->render(gRenderer, map->getVisibleLevel());
                    ballisticEngine->render(gRenderer, map->getVisibleLevel());

                    //Update screen
                    SDL_RenderPresent( gRenderer );

                    break;
                }

                /***************PAZOOK******************/
                case PAZOOK:
                    while(SDL_PollEvent( &e ) != 0 ){
                        if( e.type == SDL_QUIT ){
                            quit = true;
                        }else if(listenKeys){
                            pazookEngine->handleEvent( e );
                        }
                    }

                    //set draw color and render dialog
                    pazookEngine->render(gRenderer);
                    SDL_RenderPresent( gRenderer );

                    if(pazookEngine->isFinished()){ //if pazook is finished
                        gameState = PLAYING;
                        listenKeys = true;
                        bottomBar->startOrResumeLevelTimer();
                        if(pazookEngine->hasWon()){
                            bottomBar->incrementPazookVictory();
                            if(bottomBar->getPazookVictories() >= PAZOOK_NB_WINNING_NEEDED){
                                for(int i=0; i<map->getGameObjects()->size(); i++){
                                    EndObject * endObject = dynamic_cast<EndObject *> (map->getGameObjects()->at(i));
                                    if(endObject){
                                        endObject->setDeactivated(false);
                                    }
                                }
                            }
                            pazookEngine->reset();
                        }
                        break;
                    }else if(pazookEngine->isInSelection()){
                        listenKeys = true;
                    }
                break;
            }

            while(frameTimer.getTicks() < FRAME_TIME); //make sure it's not running too fast
            frameTimer.reset();
        }

        //Free resources
        delete map;
        delete menu;
        delete dialogPlayer;
        delete lTextureFactory;
        /*delete playerInventory; //TODO fix destructor issues
        delete bottomBar;
        delete ballisticEngine;*/
        delete pazookEngine;
        delete ballisticEngine;
        delete player;
        delete soundEngine;
	}

	//...and close SDL
	close();
}

/* Most of following code was originally from Lazy Foo' Productions (http://lazyfoo.net/) */
bool Game::init(){
    //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}else{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "This is a Little 2D Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}else{
            gRenderer = SDL_CreateRenderer( gWindow, -1, LINUX? SDL_RENDERER_SOFTWARE: SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if( gRenderer == NULL ){
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}else{
			    int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) ){
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }else {
                    //Initialize renderer color
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                    //sets icon on window
                    SDL_Surface* iconSurface = IMG_Load( "res/icon.bmp" );
                    SDL_SetWindowIcon(gWindow, iconSurface);

                    // Initialize SDL_ttf library
                    if(TTF_Init() != 0){
                      printf( "TTF_Init() Failed: " , TTF_GetError() );
                      success = false;
                    }
                    else if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) < 0 ){
                            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                            success = false;
                    }
                }
			}
		}
	}

	return success;

}

void Game::close(){

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
    Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
