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


SoundEngine::SoundEngine()
{
    if( loadMedia() ){
        SoundEngine::instance = this;
    }
    else
    {
        std::cerr << "Can't init sound engine" << std::endl;
    }
}

SoundEngine::~SoundEngine()
{
    SoundEngine::instance = nullptr;

    if(music){
        stopMusic();
    }

    Mix_FreeChunk( jump );
    jump = nullptr;

}

bool SoundEngine::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load sound effects
    jump = Mix_LoadWAV( "sound/jump.wav" );
    if( jump == NULL )
    {
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    Mix_VolumeChunk(jump, effectsVolume);

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
    Mix_Chunk * chunk;

    switch( soundEvent ){
        case JUMP:
            chunk = jump;
        break;
    }

    Mix_PlayChannel( -1, chunk, 0 );

}

SoundEngine * SoundEngine::getInstance(){
    return SoundEngine::instance;
}
