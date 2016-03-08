#include "Player.h"

Player::Player(SDL_Renderer* gRenderer, SDL_Window* gWindow, BallisticEngine* ballisEngine, PlayerInventory* inv)
 : IRenderedElement(), ballisticEngine(ballisEngine), inventory(inv) {

    initPos();

    //initialize animation textures
    gAnimRight = new LTexture(gRenderer, true);
    gAnimLeft = new LTexture(gRenderer, true);
    gJumpTextureRight = new LTexture(gRenderer, true);
    gJumpTextureLeft = new LTexture(gRenderer, true);
    gFireTextureRight = new LTexture(gRenderer, true);
    gFireTextureLeft = new LTexture(gRenderer, true);
    gAnimDeath = new LTexture(gRenderer, true);

    SDL_Color colorK = { 0xFF, 0xFF, 0xFF };

    //Load sprite sheet texture
    if( !gAnimRight->loadFromFile( "img/playerAnimRight.png", colorK) ){
        printf( "Failed to load sprite sheet texture!\n" );
    }

    if( !gAnimLeft->loadFromFile( "img/playerAnimLeft.png", colorK) ){
        printf( "Failed to load sprite sheet texture!\n" );
    }

    //Set top left sprite
    gSpriteClips[ 0 ].x = 0;
    gSpriteClips[ 0 ].y = 0;
    gSpriteClips[ 0 ].w = 30;
    gSpriteClips[ 0 ].h = 55;

    //Set top right sprite
    gSpriteClips[ 1 ].x = 30;
    gSpriteClips[ 1 ].y =  0;
    gSpriteClips[ 1 ].w = 30;
    gSpriteClips[ 1 ].h = 55;

    //Set bottom left sprite
    gSpriteClips[ 2 ].x = 60;
    gSpriteClips[ 2 ].y = 0;
    gSpriteClips[ 2 ].w = 30;
    gSpriteClips[ 2 ].h = 55;

    //Set bottom right sprite
    gSpriteClips[ 3 ].x = 90;
    gSpriteClips[ 3 ].y = 0;
    gSpriteClips[ 3 ].w = 30;
    gSpriteClips[ 3 ].h = 55;

    if( !gJumpTextureRight->loadFromFile( "img/playerJumpRight.png", colorK ) ){
        printf( "Failed to load sprite sheet texture: playerJumpRight!\n" );
    }
    if( !gJumpTextureLeft->loadFromFile( "img/playerJumpLeft.png", colorK ) ){
        printf( "Failed to load sprite sheet texture: playerJumpLeft!\n" );
    }
    if( !gFireTextureRight->loadFromFile( "img/playerFireRight.png", colorK ) ){
        printf( "Failed to load sprite sheet texture: playerFireRight!\n" );
    }
    if( !gFireTextureLeft->loadFromFile( "img/playerFireLeft.png", colorK ) ){
        printf( "Failed to load sprite sheet texture: playerFireLeft!\n" );
    }
    if( !gAnimDeath->loadFromFile( "img/playerDeathAnim.png", colorK ) ){
        printf( "Failed to load sprite sheet texture: playerDeathAnim!\n" );
    }
}

void Player::initPos(){
    //set player position based on height
    pos.x = 0;
    pos.y = SCREEN_HEIGHT - MARGIN_BOTTOM - PLAYER_HEIGHT - 1;

    //set player collider
    mCollider.y = SCREEN_HEIGHT - MARGIN_BOTTOM - PLAYER_HEIGHT;
    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;

    playerSpeed = PLAYER_INIT_SPEED;
    playerSpeedInJump = PLAYER_INIT_SPEED_IN_JUMP;
    mVelY = 0;
    mVelX = 0;
    lastVelX = 1;
}

void Player::reinit(){

    //stop timers
    dieTimer.stop();
    jumpTimer.stop();
    died = false;

    //reset velocities
    mVelX = 0;
    lastVelX = 0;
    mVelY = 0;

    init = true;

    pos.x = 0;
    pos.y = SCREEN_HEIGHT - MARGIN_BOTTOM - PLAYER_HEIGHT - 1;
}

void Player::handleEvent(SDL_Event& e){

    if(init){ //flushes all key events after init so we avoid false moves
        SDL_FlushEvent(SDL_KEYDOWN);
        SDL_FlushEvent(SDL_KEYUP);
        init = false;
    }

	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        switch( e.key.keysym.sym ){
            case SDLK_SPACE:
                if(!jumpTimer.isStarted() && mVelY <= 2){ //if not in jump and not in the air
                    mVelY = -1 * ((superJump)?PLAYER_SUPER_JUMP_VEL:PLAYER_JUMP_VEL);
                    playerSpeed = playerSpeedInJump;
                    jumpTimer.start();
                }
            break;
            case SDLK_j:
                if(inventory->hasItem(2)){ //item 2 = super jump
                    superJump = !superJump;
                    inventory->getItems().at(2)->setDisabled(!superJump); //disable/enable in inventory
                    std::cout << (superJump?"super jump activated":"super jump disabled") << std::endl;
                }
                break;
            case SDLK_LEFT: mVelX = -10; break; //just for animation
            case SDLK_RIGHT: mVelX = 10; break;
            case SDLK_f:
                if(inventory->hasItem(1) && !fireTimer.isStarted()){ //item 1 = basic gun
                    ballisticEngine->fireBullet(pos, facingLeft() ? LEFT : RIGHT);
                    fireTimer.start();
                }
        }
    }
}

//TODO clean this shitty code
void Player::move(Map* map){
    if(fireTimer.isStarted() && fireTimer.getTicks() > PLAYER_FIRE_INTERVAL){
        fireTimer.stop();
    }

    //save current player position before trying to change it
    int initPosX = pos.x;
    int initPosY = pos.y;
    int initPlayerSpeed = playerSpeed;

    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL ); //get pressed key

    //TODO clean following code

    /************ HANDLE X ***************/
    checkX:
    if(!isDying() && currentKeyStates[ SDL_SCANCODE_RIGHT ]) { //right key pressed
        pos.x += playerSpeed;
    }

    if(!isDying() && currentKeyStates[ SDL_SCANCODE_LEFT ]){ //left key pressed
        pos.x -= playerSpeed;
    }
    mCollider.x = pos.x;

    //if end level or collision
    if( ( pos.x < 0 ) || ( pos.x + PLAYER_WIDTH > map->getLevelWidth() ) || map->checkCollision(mCollider, false) ){

        SDL_Rect upCollider = mCollider;
        upCollider.y -= PLAYER_CLIMB_STEP; //retry in case of 1 pixel step

        if( !jumpTimer.isStarted() && pos.x >= 0 && pos.x + PLAYER_WIDTH < map->getLevelWidth() && !map->checkCollision(upCollider, false) ){
            pos.y = upCollider.y; //move player up (because of step)
        }else{
            pos.x = initPosX;    //revert position to initPos
            mCollider.x = pos.x;

            if(playerSpeed > 0 && !jumpTimer.isStarted()){
                playerSpeed-=2; //retry with smaller displacement
                goto checkX;
            }
        }
    }

    /************ HANDLE Y ***************/
    playerSpeed = initPlayerSpeed; //save initial displacement

    gravity(map); //(try to) move playing according to gravity

    checkY:

    pos.y += mVelY; //add vertical velocity to player position
    mCollider.y = pos.y;

    //if end level or collision
    if( ( pos.y < 0 ) || ( pos.y + PLAYER_HEIGHT > SCREEN_HEIGHT - MARGIN_BOTTOM ) || map->checkCollision(mCollider, false) ){
        pos.y = initPosY; //revert to initial position
        mCollider.y = pos.y;

        if(jumpTimer.isStarted()){ //if in jump

            if(mVelY < 0){ //if going up
                mVelY = -1 * mVelY; //invert vertical velocity
            }else{ //going down
                jumpTimer.stop(); //end of jump (hit ground)
                playerSpeed=PLAYER_INIT_SPEED;
                goto checkY;
            }
        }else if(mVelY>0){ //if falling
            //mVelY=1;
            if(mVelY >= 1){
                mVelY--;
                goto checkY;
            }else{
                mVelY-=0.2;
                goto checkY;
            }
        }
    }
}

void Player::gravity(Map* map){

    SDL_Rect newPlayerPos = mCollider;

    newPlayerPos.y = pos.y + 1; //create anticipatory shift

    if(( newPlayerPos.y + PLAYER_HEIGHT < SCREEN_HEIGHT - MARGIN_BOTTOM ) && !map->checkCollision(newPlayerPos, false)){
        mVelY += WORLD_FALL_ACC; //add fall acceleration if nothing below
    }
}

void Player::render(SDL_Renderer* gRenderer, const SDL_Rect &mapVisibleLevel){

    if(dieTimer.isStarted()){ //play death animation
        if(dieTimer.getTicks() > PLAYER_DIE_TIME){
            died = true;
        }else if(dieTimer.getTicks() > frame * (PLAYER_DIE_TIME/4) + (PLAYER_DIE_TIME/4)){
            frame = (frame + 1) % 4; //change frame
        }
        SDL_Rect* currentClip = &gSpriteClips[ frame ];
        gAnimDeath->render( pos.x - mapVisibleLevel.x, pos.y, currentClip );
        return;
    }

    if(fireTimer.isStarted() && fireTimer.getTicks() < PLAYER_FIRE_ANIM_TIME){ //play fire animation
        LTexture* fireAnim;
        if(facingLeft()){
            fireAnim = gFireTextureLeft;
        }else{
            fireAnim = gFireTextureRight;
        }

        fireAnim->render(pos.x - mapVisibleLevel.x, pos.y);

        SDL_RenderPresent( gRenderer );
        return;
    }else if(jumpTimer.isStarted()){ //render with jump texture
        LTexture* jumpAnim;
        if(facingLeft()){
            jumpAnim = gJumpTextureLeft;
        }else{
            jumpAnim = gJumpTextureRight;
        }

        jumpAnim->render(pos.x - mapVisibleLevel.x, pos.y);

        SDL_RenderPresent( gRenderer );
        return;
    }

    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL ); //get pressed key

    if( currentKeyStates[ SDL_SCANCODE_RIGHT ] || currentKeyStates[ SDL_SCANCODE_LEFT ]){
        if(!walkTimer.isStarted()){
            walkTimer.start();
        }

        if(walkTimer.getTicks() > PLAYER_FIRE_ANIM_TIME){
            frame++;

            //Cycle animation
            if( frame >= WALKING_ANIMATION_FRAMES ){
                frame = 0;
            }
            walkTimer.reset();
        }
    }

    SDL_Rect* currentClip = &gSpriteClips[ frame ];

    LTexture* anim = gAnimRight;

    if(facingLeft()){
        anim = gAnimLeft;
    }

    anim->render( pos.x - mapVisibleLevel.x, pos.y, currentClip );
}


void Player::die(){
    dieTimer.start(); //start death animation
    frame = 0;
}

bool Player::isDying(){
    return dieTimer.isStarted();
}

bool Player::hasDied(){
    return died;
}

PlayerPosition* Player::getPos(){
    return &pos;
}

Player::~Player(){
    delete gAnimRight;
    delete gAnimLeft;
    delete gJumpTextureRight;
    delete gJumpTextureLeft;
    delete gFireTextureLeft;
    delete gFireTextureRight;
    delete gAnimDeath;
}

bool Player::facingLeft(){
    if(mVelX < 0){
        return true;
    }else if(mVelX > 0){
        return false;
    }else if(lastVelX < 0){
         return true;
    }else if(lastVelX >= 0){
        return false;
    }
    /*return mVelX < 0 || lastVelX < 0;*/
}
