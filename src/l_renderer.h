#ifndef LRENDERER_H8
#define LRENDERER_H

#include <SDL.h>


class LRenderer
{
private:
	SDL_Renderer* mRenderer;

public:
	//bool createFromWindow(const LWindow& window);
	bool createFromGWindow();
	void free();

	SDL_Texture* createTextureFromSurface(SDL_Surface* surface) const;
	SDL_Texture* createTextureBlank(const int width, const int height, SDL_TextureAccess access) const;

	void clear() const;
	void copy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) const;
	void copyEx(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect,
					const double angle, const SDL_Point* center, const SDL_RendererFlip flip) const;
	void present() const;

	void DrawRect(const SDL_Rect* rect) const;

	void setDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) const;
	void setTargetTexture(SDL_Texture* texture) const;
	void unsetTargetTexture() const;

	void renderReadPixels(SDL_Surface* surface, SDL_Rect* capture_rect = NULL) const;
};

#endif // LRENDERER_H
