//#include <SDL_image.h>//

#include "l_window.h"
#include "l_renderer.h"


extern LWindow gWindow;

bool LRenderer::createFromGWindow()
{
	mRenderer = gWindow.createRenderer();
	if (mRenderer == nullptr)
		return false;
	else
	{
		SDL_SetRenderDrawColor( mRenderer, 0x00, 0x00, 0x00, 0xFF );//black
		return true;
	}
}
void LRenderer::free()
{
	SDL_DestroyRenderer( mRenderer );
	mRenderer = nullptr;
}

SDL_Texture* LRenderer::createTextureFromSurface(SDL_Surface* surface) const
{
	return SDL_CreateTextureFromSurface( mRenderer, surface );
}
SDL_Texture* LRenderer::createTextureBlank(const int width, const int height, SDL_TextureAccess access) const
{
	return SDL_CreateTexture( mRenderer, SDL_PIXELFORMAT_RGBA8888, access, width, height );
}

void LRenderer::clear() const
{
	SDL_RenderClear( mRenderer );
}
void LRenderer::copy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) const
{
	SDL_RenderCopy(mRenderer, texture, srcrect, dstrect);
}
void LRenderer::copyEx(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect,
					const double angle, const SDL_Point* center, const SDL_RendererFlip flip) const
{
	SDL_RenderCopyEx( mRenderer, texture, srcrect, dstrect, angle, center, flip );
}
void LRenderer::present() const
{
	SDL_RenderPresent( mRenderer );
	if (gWindow.isDebugScreen())
	{
		gWindow.updateFps();
	}
}

void LRenderer::DrawRect(const SDL_Rect* rect) const
{
	SDL_RenderDrawRect(mRenderer, rect);
}

void LRenderer::setDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) const
{
	SDL_SetRenderDrawColor(mRenderer, r, g, b, a);
}
void LRenderer::setTargetTexture(SDL_Texture* texture) const
{
	SDL_SetRenderTarget( mRenderer, texture );
}
void LRenderer::unsetTargetTexture() const
{
	SDL_SetRenderTarget( mRenderer, nullptr );
}

void LRenderer::renderReadPixels(SDL_Surface* surface, SDL_Rect* capture_rect) const
{
	//crashes if surface is smaller than capture_rect(or default render terget, if capture_rect is NULL)
	SDL_RenderReadPixels(mRenderer, capture_rect, SDL_PIXELFORMAT_RGBA8888, surface->pixels, surface->pitch);
}
