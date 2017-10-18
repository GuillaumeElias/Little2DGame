#include "Utils/LMapTexture.h"

LMapTexture::LMapTexture(SDL_Renderer* renderer, SDL_Window* window) : LTexture(renderer, true){
    gWindow = window;
}

LMapTexture::~LMapTexture(){

}

/* Most of following code was originally from Lazy Foo' Productions (http://lazyfoo.net/) */
bool LMapTexture::loadFromFile( std::string path, SDL_Color colorKey ){
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

    if( loadedSurface == NULL ){
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else{
         //Convert surface to display format
        SDL_Surface* formattedSurface = SDL_ConvertSurface( loadedSurface, SDL_GetWindowSurface( gWindow )->format, NULL );
        if( formattedSurface == NULL ){
            printf( "Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError() );
        }else{
            //Color key image
            SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, colorKey.r, colorKey.g, colorKey.b ) );

            //Create texture from surface pixels
            newTexture = SDL_CreateTexture( gRenderer, SDL_GetWindowPixelFormat( gWindow ), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
            if( newTexture == NULL ){
                printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }else{

                 //Lock texture for manipulation
                SDL_LockTexture( newTexture, NULL, &mPixels, &mPitch );

                //Copy loaded/formatted surface pixels
                memcpy( mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );

                //Unlock texture to update
                SDL_UnlockTexture( newTexture );
                mPixels = NULL;

                //Get image dimensions
                mWidth = formattedSurface->w;
                mHeight = formattedSurface->h;

            }
        }

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LMapTexture::lockTexture(){
    bool success = true;

    //Texture is already locked
    if( mPixels != NULL ){
        printf( "Texture is already locked!\n" );
        success = false;
    }
    //Lock texture
    else{
        if( SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch ) != 0 ){
            printf( "Unable to lock texture! %s\n", SDL_GetError() );
            success = false;
        }
    }

    return success;
}

bool LMapTexture::unlockTexture(){
    bool success = true;

    //Texture is not locked
    if( mPixels == NULL ){
        printf( "Texture is not locked!\n" );
        success = false;
    }
    //Unlock texture
    else{
        SDL_UnlockTexture( mTexture );
        mPixels = NULL;
        mPitch = 0;
    }

    return success;
}

void* LMapTexture::getPixels(){
    return mPixels;
}

int LMapTexture::getPitch(){
    return mPitch;
}

Uint32 LMapTexture::getPixel(int x, int y ){
    //Get the requested pixel
    //return mPixels[ ( y * mPitch ) + x ];
    return NULL;
}
