#include "SoundEngine.h"
#include <iostream>
#include <sstream>
#include <cstdio>
#include <SDL.h>

SoundEngine * SoundEngine::instance = nullptr;
Mix_Music * SoundEngine::music = nullptr;
int SoundEngine::effectsVolume = 100;
int SoundEngine::musicVolume = MIX_MAX_VOLUME / 4;
int SoundEngine::currentLevel = -1;


std::string soundEventToString(SoundEvent event){
    switch (event)
    {
        case JUMP: return "jump";
        case SUPERJUMP: return "superjump";
        case CLICK: return "click";
        case CLICK_UP: return "clickUp";
        case CLICK_DOWN: return "clickDown";
        case FIRE: return "fire";
        case ITEM: return "item";
        case BLOB_DEATH: return "blobDeath";
        case ZOMBIE_DEATH: return "zombieDeath";
        case BEAST_DEATH: return "beastDeath";
        case PLAYER_DEATH: return "playerDeath";
        case HURT: return "hurt";
        case BANANA: return "banana";
        case PAZOOK_WIN: return "pazookWin";
        case PAZOOK_LOSE: return "pazookLose";
        case SPAWN: return "spawn";
    }
}

std::string getPathForSoundEvent(SoundEvent event){
    std::ostringstream path;
    path << "sound/" << soundEventToString(event) << ".wav";
    return path.str();
}

SoundEngine::SoundEngine()
{
    soundEvents = {
        { JUMP, NULL },
        { SUPERJUMP, NULL },
        { CLICK, NULL },
        { CLICK_UP, NULL },
        { CLICK_DOWN, NULL },
        { FIRE, NULL },
        { ITEM, NULL },
        { BLOB_DEATH, NULL },
        { ZOMBIE_DEATH, NULL },
        { BEAST_DEATH, NULL },
        { HURT, NULL },
        { PLAYER_DEATH, NULL },
        { BANANA, NULL },
        { PAZOOK_WIN, NULL },
        { PAZOOK_LOSE, NULL },
        { SPAWN, NULL }
    };

    if( loadMedia() ){
        SoundEngine::instance = this;
    }
    else
    {
        std::cerr << "Can't init sound engine" << std::endl;
    }
}

bool SoundEngine::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load sound effects
    for(auto keyValue : soundEvents){
        SoundEvent event = keyValue.first;

        std::string soundPath = getPathForSoundEvent(event);
        soundEvents[event] = Mix_LoadWAV( soundPath.c_str() );
        if( soundEvents[event] == NULL )
        {
            printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
            return false;
        }

        Mix_VolumeChunk(soundEvents[event], effectsVolume);
    }

    return success;
}


void SoundEngine::startMusic(int level){
    SoundEngine::currentLevel = level;

    if( Mix_PlayingMusic() != 0 ){
        Mix_FadeOutMusic( MUSIC_FADE_OUT );
        Mix_HookMusicFinished(&SoundEngine::startNextMusic);
    }else{
        startNextMusic();
    }
}

void SoundEngine::startNextMusic(){
    if(music != NULL){
        stopMusic();
    }

    std::ostringstream path;
    path << "sound/music/level" << currentLevel << ".mp3";

    music = Mix_LoadMUS( path.str().c_str() );
    if( music == NULL )
    {
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        return;
    }

    Mix_VolumeMusic( musicVolume );
    Mix_FadeInMusic( music, -1, MUSIC_FADE_IN );
}

void SoundEngine::pauseMusic(){
    Mix_PauseMusic();
}

void SoundEngine::resumeMusic(){
    Mix_ResumeMusic();
}

void SoundEngine::stopMusic(){

    Mix_HaltMusic();

    if(music){
        Mix_FreeMusic( music );
        music = nullptr;
    }else{
        std::cerr << "music was null" << std::endl;
    }
}

void SoundEngine::soundEvent(SoundEvent soundEvent){
    if(!this || soundEvents.empty()) //yes, I know...
    {
        return;
    }

    Mix_Chunk * chunk = soundEvents[soundEvent];
    Mix_PlayChannel( -1, chunk, 0 );
}

SoundEngine * SoundEngine::getInstance(){
    return SoundEngine::instance;
}

SoundEngine::~SoundEngine()
{
    SoundEngine::instance = nullptr;

    if(music){
        stopMusic();
    }

    for (auto keyValue : soundEvents) {
        Mix_FreeChunk( keyValue.second );
        soundEvents[keyValue.first] = nullptr;
    }

    soundEvents.clear();

}
