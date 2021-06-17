#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <string>
#include <SDL.h>


class LTexture
{
private:
	//The actual hardware texture
	SDL_Texture* mTexture;
	//Image dimensions
	int mWidth;
	int mHeight;

public:
	//Initializes variables
	LTexture();
	//Deallocates memory
	~LTexture();
	//Loads image at specified path
	bool loadFromFile( const char* path );
	//#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
	//Creates image from font string
	bool loadFromRenderedText( const char* textureText, SDL_Color textColor );
	bool loadFromRenderedText( const Uint16* textureText, SDL_Color textColor );
	//#endif
	//Creates blank texture(from LazyFoo 43)
	bool createBlank(const int width, const int height, SDL_TextureAccess = SDL_TEXTUREACCESS_STREAMING);
	//Deallocates texture
	void free();

	//Set color modulation
	void setColor( const Uint8 red, const Uint8 green, const Uint8 blue );
	//Set blending
	void setBlendMode(SDL_BlendMode blending = SDL_BLENDMODE_BLEND);
	//Set alpha modulation
	void setAlpha( const Uint8 alpha );

	//Wrapper around RenderCopy's
	void render(const int x, const int y, const SDL_Rect* clip = nullptr) const;
	void renderEx(const int x, const int y, const SDL_Rect* clip = nullptr, const double angle = 0.0,
				const SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	void renderDest(const SDL_Rect* clip, const SDL_Rect* dstrect) const;

	//Set self as render target(from LazyFoo 43)
	void setAsRenderTarget() const;
	void resetRenderTarget() const;
	//Gets image dimensions
	int getWidth()  const { return mWidth; }
	int getHeight() const { return mHeight; }

	SDL_Texture* getSDLTexture() const { return mTexture; }
};

#endif // LTEXTURE_H
