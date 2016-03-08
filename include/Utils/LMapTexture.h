#ifndef LMAPTEXTURE_H
#define LMAPTEXTURE_H

#include <SDL.h>
#include <Utils/LTexture.h>

/**
Texture for map. Allows access to individual pixels.
*/
class LMapTexture : public LTexture
{
    public:
        LMapTexture(SDL_Renderer* renderer, SDL_Window* gWindow);
        virtual ~LMapTexture();

        virtual bool loadFromFile( std::string path, SDL_Color colorKey );

        bool lockTexture(); //the texture needs to be locked before accessing pixels
        bool unlockTexture();

        void* getPixels();
        int getPitch();
        Uint32 getPixel(int x, int y);
    protected:
    private:
        SDL_Window* gWindow;
};

#endif // LMAPTEXTURE_H
