#ifndef LTEXTURE_H
#define LTEXTURE_H
#include <string>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <../src/Constants.h>

/**
Texture for all objects.
*/
class LTexture{
	public:
		LTexture(SDL_Renderer* renderer, bool inGameViewport);

		~LTexture();

		virtual bool loadFromFile( std::string path, SDL_Color colorKey );

		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		void free();

		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		void setBlendMode( SDL_BlendMode blending );

		void setAlpha( Uint8 alpha );

		void render( int x, int y, const SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		int getWidth();
		int getHeight();
		void setScaling(float scaling);
		float getScaling();

		void setInGameViewPort(bool inGame); //set if in game or not

	protected:
		SDL_Texture* mTexture;
		SDL_Renderer* gRenderer;

        bool inGameViewport;

        void* mPixels;
        int mPitch;

		int mWidth;
		int mHeight;
		float mScaling;
};

#endif // LTEXTURE_H
