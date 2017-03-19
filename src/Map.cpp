#include "Map.h"

bool isWall(Uint32 pixel);

Map::Map(SDL_Renderer* renderer, SDL_Window* window, BottomBar* bottomBar, LTextureFactory* lTextureFactory, PlayerInventory* playerInventory)
: gRenderer(renderer), gWindow(window), bottomBar(bottomBar), lTextureFactory(lTextureFactory), playerInventory(playerInventory){

    //loadLevel(0);
}

void Map::loadLevel(int nb){

    //build and load level texture
    gLevelTexture = new LMapTexture(gRenderer, gWindow);

    std::ostringstream levelPathBaseOss;
    levelPathBaseOss << "levels/level" << nb;

    if( !gLevelTexture->loadFromFile(levelPathBaseOss.str() + ".png", {0, 0, 0} ) ){
        printf( "Failed to load level texture!\n" );
        return;
    }

    int totalBananasInLevel = 0;
    //read level descriptor text file
    std::ifstream txtFile(std::string(levelPathBaseOss.str() + ".txt").c_str());
    std::string line;
    while (std::getline(txtFile, line)){ //each line creates a new instance of a game object
        std::istringstream iss(line);
        int posX, posY, param;
        std::string typeStr;
        if (!(iss >> posX >> posY >> typeStr >> param)) {
            std::cerr << "Could not read line" << std::endl;
            break;
        }

        //create specific game object instance based on type
        if(typeStr == "BLOB"){
            BlobObject* blob = new BlobObject(gRenderer, gWindow, bottomBar, posX, posY, lTextureFactory);
            blob->init();
            gameObjects.push_back(blob);
        }else if(typeStr == "ZOMBIE"){
            ZombieObject* zombie = new ZombieObject(gRenderer, gWindow, bottomBar, posX, posY, lTextureFactory, param);
            zombie->init();
            gameObjects.push_back(zombie);
        }else if(typeStr == "BEAST"){
            BeastObject* beast = new BeastObject(gRenderer, gWindow, bottomBar, posX, posY, lTextureFactory, this, &Map::checkCollision);
            beast->init();
            gameObjects.push_back(beast);
        }else if(typeStr == "ELEVATOR"){
            ElevatorObject* elevator = new ElevatorObject(gRenderer, gWindow, posX, posY, lTextureFactory, param);
            elevator->init();
            gameObjects.push_back(elevator);
        }else if(typeStr == "CHARACTER"){
            Character* characterObj = new Character(gRenderer, gWindow, posX, posY, lTextureFactory, param, &gameObjects);
            characterObj->init();
            gameObjects.push_back(characterObj);
            characters[param] = characterObj;
        }else if(typeStr == "END"){
            EndObject* endObj = new EndObject(gRenderer, gWindow, bottomBar, posX, posY, lTextureFactory, param);
            endObj->init();
            gameObjects.push_back(endObj);
        }else if(typeStr == "TRIGGER"){
            Trigger* triggerObj = new Trigger(gRenderer, gWindow, posX, posY, lTextureFactory, param);
            triggerObj->init();
            gameObjects.push_back(triggerObj);

            if(param >= 100 && characters.count(param - TRIGGER_CHARACTER_PARAM_MASK) > 0){
                triggerObj->setCharacter(characters[param - TRIGGER_CHARACTER_PARAM_MASK]);
            }
        }else if(typeStr == "ITEM"){
            Item* item = new Item(gRenderer, gWindow, posX, posY, lTextureFactory, param, playerInventory, &PlayerInventory::addItem);
            item->init();
            gameObjects.push_back(item);
        }else if(typeStr == "BANANA"){
            BananaObject* bananaObj = new BananaObject(gRenderer, gWindow, bottomBar, posX, posY, lTextureFactory, &gameObjects);
            bananaObj->init();
            gameObjects.push_back(bananaObj);
            totalBananasInLevel++;
        }
    }

    levelPathBaseOss.clear();

    bottomBar->setTotalBananas(totalBananasInLevel);

    resetLevel();
}

void Map::resetLevel(){
    visibleLevel.x = 0;
    visibleLevel.y = 0;
    visibleLevel.w = SCREEN_WIDTH;
    visibleLevel.h = SCREEN_HEIGHT - MARGIN_BOTTOM;

    //call rebirth() for each game object
    for(int i=0; i<gameObjects.size(); i++){
        gameObjects[i]->rebirth();
    }
}

void Map::unloadLevel(){

    gameObjects.clear();
    characters.clear();

    gLevelTexture->free();

    delete gLevelTexture;
}

void Map::handleEvent( SDL_Event& e ){
    //TODO
}

void Map::move(PlayerPosition* pPos){

    int midScreen = visibleLevel.x + SCREEN_WIDTH / 2;

    if(pPos->x > midScreen && (visibleLevel.x + visibleLevel.w) < gLevelTexture->getWidth()){
        visibleLevel.x += pPos->x - midScreen;
    }

    if(pPos->x < midScreen && visibleLevel.x > 0){
         visibleLevel.x += pPos->x - midScreen;
    }
}

bool Map::checkCollision(const SDL_Rect &collider, bool terrainOnly){
    int yShift = (collider.y > 0)? 1 : 0;

    int topLeftX = collider.x;
    int topLeftY = collider.y - yShift;

    int topRightX = collider.x + collider.w;
    int topRightY = collider.y - yShift;

    int bottomLeftX = collider.x;
    int bottomLeftY = collider.y + collider.h - 1;

    int bottomRightX = collider.x + collider.w;
    int bottomRightY = collider.y + collider.h - 1;

    gLevelTexture->lockTexture();

    //Get pixel data
    Uint32* pixels = (Uint32*)gLevelTexture->getPixels();

    bool collision = false;

    //top side
    for(int i=topLeftX; i <= topRightX; i++){
        Uint32 pixel = getPixel(pixels, i, topLeftY);
        if(isWall(pixel)){
            collision = true;
            goto leave;
        }
    }

     //bottom side
    for(int i=bottomLeftX; i <= bottomRightX; i++){
        Uint32 pixel = getPixel(pixels, i, bottomLeftY);
        if(isWall(pixel)){
            collision = true;
            goto leave;
        }
    }

     //left side
    for(int i=topLeftY; i <= bottomLeftY; i++){
        Uint32 pixel = getPixel(pixels, topLeftX, i);
        if(isWall(pixel)){
            collision = true;
            goto leave;
        }
    }

     //right side
    for(int i=topRightY; i <= bottomRightY; i++){
        Uint32 pixel = getPixel(pixels, topRightX, i);
        if(isWall(pixel)){
            collision = true;
            goto leave;
        }
    }

    if(!terrainOnly){
        //game objects
        for(int i=0; i<gameObjects.size(); i++){
            if(gameObjects[i]->checkCollision(collider, false)){
                collision = true;
                goto leave;
            }
        }
    }

    leave:
    gLevelTexture->unlockTexture();

    return collision;

}

void Map::render(SDL_Renderer* gRenderer, const SDL_Rect &passedVisibleLevel){
    //render level texture
    gLevelTexture->render( 0, 0, &visibleLevel );

    //render all game objects
    for(int i=0; i<gameObjects.size(); i++){
        gameObjects[i]->render(gRenderer, visibleLevel);
    }
}

SDL_Rect Map::getVisibleLevel(){
    return visibleLevel;
}

int Map::getLevelWidth(){
    return gLevelTexture->getWidth();
}

std::vector<GameObject*>* Map::getGameObjects(){
    return &gameObjects;
}

Character* Map::getCharacter(int characterId){
   if(characters.count(characterId) > 0){ //if character is in map
        return characters[characterId];
   }else{
        return NULL;
   }
}

BottomBar* const Map::getBottomBar(){
    return bottomBar;
}

Map::~Map(){
    unloadLevel();
}

Uint32 Map::getPixel(Uint32* pixels, int x, int y){
    return pixels[ ( y * getLevelWidth() ) + x ];
}

bool isWall(Uint32 pixel){
    Uint8 *colors = (Uint8*)&pixel;

    Uint8 red, green, blue;
    blue = colors[ 0 ];
    green = colors[ 1 ];
    red = colors[ 2 ];

    return (red==0 && green==0 && blue==0) || (red==195 && green==195 && blue==195) //
    || (red==237 && green==28 && blue==36)|| (red==185 && green==122 && blue==87); //brown (#B97A57)
}
