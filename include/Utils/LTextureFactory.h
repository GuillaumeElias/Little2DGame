#ifndef LTEXTUREFACTORY_H
#define LTEXTUREFACTORY_H

#include <map>
#include <SDL.h>
#include <LTexture.h>

/**
Builds and keeps all textures to avoid reloading the same texture multiple times.
*/
class LTextureFactory
{
    public:
        LTextureFactory(SDL_Renderer* gRenderer);
        virtual ~LTextureFactory();

        LTexture* fetchTexture(std::string path, bool inGameViewport);
    protected:
    private:
        std::map<std::string, LTexture*> textures;
        SDL_Renderer* gRenderer;
};

#endif // LTEXTUREFACTORY_H
