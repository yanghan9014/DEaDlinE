#include <SDL_image.h>//
#include <SDL_ttf.h>//
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>

#include <stdio.h>

#include "l_texture.h"
#include "l_renderer.h"

extern LRenderer gRenderer;//
extern TTF_Font* gFont;//


LTexture::LTexture()
{
	//Initialize
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( const char* path )
{
	if (!path)
	{
		printf("pointer to path is nullptr!\n");
		return false;
	}

	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = nullptr;
	//Load image at specified path
	//printf("[LTexture::loadFromFile] loading from %s\n", path);//TODO: logging
	SDL_Surface* loadedSurface = IMG_Load( path );

	if( loadedSurface == nullptr )
	{
		printf( "Unable to load image from %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = gRenderer.createTextureFromSurface(loadedSurface);
		if( newTexture == nullptr )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
	//Return success
	mTexture = newTexture;
	return (mTexture != nullptr);
}

//#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
bool LTexture::loadFromRenderedText( const char* textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended( gFont, textureText, textColor );
	if( textSurface != nullptr )
	{
		//Create texture from surface pixels
		//printf("[LTexture::loadFromRenderedText] creating texture of text \"%s\"\n", textureText);
        mTexture = gRenderer.createTextureFromSurface(textSurface);//

		if( mTexture == nullptr )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	//Return success
	return (mTexture != nullptr);
}
bool LTexture::loadFromRenderedText( const Uint16* textureText, SDL_Color textColor )
{
	free();

	SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(gFont, textureText, textColor);
	if( textSurface != nullptr )
	{
		printf("[LTexture::loadFromRenderedText] creating texture of text \"%s\"\n", textureText);//TODO: fix %s taking uint16
        mTexture = gRenderer.createTextureFromSurface(textSurface);//

		if( mTexture == nullptr )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	return (mTexture != nullptr);
}
//#endif

//from LazyFoo 43
bool LTexture::createBlank( const int width, const int height, SDL_TextureAccess access )
{
    //Create uninitialized texture
	//printf("[LTexture::createBlank] creating blank texture\n");
    mTexture = gRenderer.createTextureBlank(width, height, access);//

    if( mTexture == nullptr )
    {
        printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
    }
    else
    {
        mWidth = width;
        mHeight = height;
    }

    return (mTexture != nullptr);
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != nullptr )//TODO: some free() contains null guard, make consistent
	{
		SDL_DestroyTexture( mTexture );
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;

		//printf("[LTexture::free] freed texture\n");//TODO: logging switch
	}
}

void LTexture::setColor( const Uint8 red, const Uint8 green, const Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( const Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render(const int x, const int y, const SDL_Rect* clip) const
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != nullptr )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//Render to screen
	gRenderer.copy(mTexture, clip, &renderQuad);//
}
void LTexture::renderEx(const int x, const int y, const SDL_Rect* clip,
						const double angle, const SDL_Point* center, const SDL_RendererFlip flip ) const
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != nullptr )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//Render to screen
	gRenderer.copyEx(mTexture, clip, &renderQuad, angle, center, flip );
}
void LTexture::renderDest(const SDL_Rect* clip, const SDL_Rect* dstrect) const
{
	gRenderer.copy(mTexture, clip, dstrect);
}
void LTexture::setAsRenderTarget() const
{
	gRenderer.setTargetTexture(mTexture);
}
void LTexture::resetRenderTarget() const
{
	gRenderer.unsetTargetTexture();
}
