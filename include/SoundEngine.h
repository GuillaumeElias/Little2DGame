#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <map>
#include <SDL_mixer.h>
#include "../src/Constants.h"

enum SoundEvent{ JUMP, SUPERJUMP, CLICK, CLICK_UP, CLICK_DOWN, FIRE, ITEM, BLOB_DEATH, ZOMBIE_DEATH, BEAST_DEATH, PLAYER_DEATH, HURT, BANANA, PAZOOK_WIN, PAZOOK_LOSE, SPAWN };

class SoundEngine
{
    public:
        SoundEngine();
        virtual ~SoundEngine();

        void startMusic(int level);
        void pauseMusic();
        void resumeMusic();
        static void stopMusic();

        void soundEvent(SoundEvent soundEvent);

        static SoundEngine * getInstance();
    protected:
    private:
        bool loadMedia();

        static Mix_Music * music; //static to allow callback functions to access it
        std::map<SoundEvent, Mix_Chunk * > soundEvents;

        static int effectsVolume, musicVolume;
        static void startNextMusic();
        static int currentLevel;
        static SoundEngine * instance;
};

#endif // SOUNDENGINE_H
