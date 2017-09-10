#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <SDL_mixer.h>
#include "../src/Constants.h"

enum SoundEvent{ JUMP };

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
        Mix_Chunk * jump = NULL;

        static int effectsVolume, musicVolume;
        static void startNextMusic();
        static int currentLevel;
        static SoundEngine * instance;
};

#endif // SOUNDENGINE_H
