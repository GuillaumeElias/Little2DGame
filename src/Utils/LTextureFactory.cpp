#include "LTextureFactory.h"

LTextureFactory::LTextureFactory(SDL_Renderer* gRenderer) : gRenderer(gRenderer){
    //ctor
}

LTextureFactory::~LTextureFactory(){
    //TODO iterate on all texture and delete
}

LTexture* LTextureFactory::fetchTexture(std::string path, bool inGameViewport){
    if(textures.count(path) > 0){ //if present in map
        return textures[path];
    }
    //load texture if not present
    else{
        std::cout << "build and load texture:" << path << std::endl;

        LTexture* texture = new LTexture(gRenderer, true);

        if( !texture->loadFromFile(path.c_str(), {255, 255, 255} ) ){
            std::cerr <<  "Failed to load texture: "  << path << std::endl ;
            return NULL;
        }
        textures[path] = texture;

        return texture;
    }
}
