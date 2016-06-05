#include "Game.h"


Game::Game(){

    //initialize game view-port
    topViewport.x = 0;
    topViewport.y = 0;
    topViewport.w = SCREEN_WIDTH;
    topViewport.h = SCREEN_HEIGHT - MARGIN_BOTTOM;

    level = -1;
    listenKeys = true;

    //initialize game state
    gameState = MENU/*PLAYING*/; //TODO put back
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
        int triggerId = 0;

        SDL_Event e;

        //initialize game engines
        Menu* menu = new Menu(gRenderer, gWindow);
        DialogPlayer* dialogPlayer = new DialogPlayer(gRenderer, gWindow);
        LTextureFactory* lTextureFactory = new LTextureFactory(gRenderer);

        PlayerInventory* playerInventory = new PlayerInventory();
        BottomBar* bottomBar = new BottomBar(playerInventory);
        Map* map = new Map(gRenderer, gWindow, bottomBar, lTextureFactory, playerInventory);
        BallisticEngine* ballisticEngine = new BallisticEngine(gRenderer);
        Player* player = new Player(gRenderer, gWindow, ballisticEngine, playerInventory);

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
                        if(level == -1){
                            dialogPlayer->loadLevel(level++); // load intro
                            gameState = DIALOG;
                        }else{
                            menu->setMode(START);
                            gameState = PLAYING;
                            player->setPause(false);
                        }
                        menu->clearChoice();

                        break;
                    }else if(menu->getChoice() == LEAVE){ //player selected "LEAVE"
                        quit = true;
                        break;
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
                        gameState = PLAYING;
                        listenKeys = true;
                        break;
                    }

                break;

                /***************PLAYING******************/
                case PLAYING:
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

                            if(triggerId > TRIGGER_CHARACTER_PARAM_MASK){ //if the trigger is linked to a character
                                listenKeys = false;
                                Character* character = map->getCharacter(triggerId - TRIGGER_CHARACTER_PARAM_MASK); //fetch character

                                if(character != NULL && dialogPlayer->loadCharacterDialog(level, character)){
                                     gameState = DIALOG;
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
                        }

                    }else if(bottomBar->isLevelFinished()){ //if level finished
                        listenKeys = false;
                        player->reinit(); //reset player position
                        bottomBar->rebirth(); //reset bottom bar
                        map->unloadLevel();
                        dialogPlayer->loadLevel(level); //load new level dialog
                        map->loadLevel(++level); //load to next level
                        gameState = DIALOG; //do to dialog mode

                        continue;
                    }else if(player->inPause()){
                        if(menu->getMode() != PAUSE){
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

            while(frameTimer.getTicks() < FRAME_TIME); //make sure it's not running too fast
            frameTimer.reset();
        }

	}

	//Free resources and close SDL
	close();
}

bool Game::init(){
    //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}else{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Little 2D Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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

                    // Initialize SDL_ttf library
                    if(TTF_Init() != 0){
                      printf( "TTF_Init() Failed: " , TTF_GetError() );
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
	IMG_Quit();
	SDL_Quit();
}
