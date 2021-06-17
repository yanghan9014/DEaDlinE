#include "overlay.h"
#include "scene.h"
#include "l_window.h"//bad
extern LWindow gWindow;


Overlay::Overlay(Scene* scene, const char* filepath, EffectType effect,
				 const float depth, const int originX, const int originY)
	: SceneObject(scene)
{
	mLTexture.loadFromFile(filepath);

	mAlpha = 0;
	mDeltaAlpha = 0;
	mEffect = effect;
	if (mEffect & STATIC)
		mNoScale = true;
	mKeepAfterFade = true;

	mDepth = depth;
	mOriginX = originX;
	mOriginY = originY;
}
void Overlay::render()
{
	fade();
	switch (mEffect)
	{
	case NONE:
		mLTexture.render((int)(0 - mOriginX - mSubscribedScene->mCamera.x / mDepth),
						 (int)(0 - mOriginY - mSubscribedScene->mCamera.y / mDepth));
		break;

	case STATIC:
		{
			if (gWindow.isFullScreen())
			{
				SDL_Rect srcrect = {0, 0, mLTexture.getWidth(), mLTexture.getHeight()};
				SDL_Rect dstrect = {mOriginX, mOriginY,
									gWindow.getFullScreenRect().w, gWindow.getFullScreenRect().h};
				mLTexture.renderDest(&srcrect, &dstrect);
			}
			else
				mLTexture.render(mOriginX, mOriginY);
		}
		break;

	case STRETCH:
		{
			SDL_Rect srcrect = {0, 0, mLTexture.getWidth(), mLTexture.getHeight()};
			SDL_Rect dstrect = {0, 0, mSubscribedScene->mCamera.w, mSubscribedScene->mCamera.h};
			mLTexture.renderDest(&srcrect, &dstrect);
		}
		break;

	case (STATIC | STRETCH):
		{
			if (gWindow.isFullScreen())
			{
				SDL_Rect srcrect = {0, 0, mLTexture.getWidth(), mLTexture.getHeight()};
				SDL_Rect dstrect = {mOriginX, mOriginY,
									gWindow.getFullScreenRect().w, gWindow.getFullScreenRect().h};
				mLTexture.renderDest(&srcrect, &dstrect);
			}
			else
			{
				SDL_Rect srcrect = {0, 0, mLTexture.getWidth(), mLTexture.getHeight()};
				SDL_Rect dstrect = {0, 0, WINDOWED_WIDTH, WINDOWED_HEIGHT};
				mLTexture.renderDest(&srcrect, &dstrect);
			}
		}
		break;


	}
}

void Overlay::setColor(const Uint8 red, const Uint8 green, const Uint8 blue)
{
	mLTexture.setColor(red, green, blue);
}
void Overlay::setBlendMode(SDL_BlendMode blending)
{
	mLTexture.setBlendMode();
}
void Overlay::setAlpha(const Uint8 alpha)
{
	mLTexture.setAlpha(alpha);
}

void Overlay::setFade(const unsigned char start_alpha, const char delta_alpha, bool keep)
{
	setBlendMode(SDL_BLENDMODE_BLEND);

	mAlpha = start_alpha;
	mDeltaAlpha = delta_alpha;

	mKeepAfterFade = keep;
}
void Overlay::fade()
{
	if (mDeltaAlpha)
	{
		setAlpha(mAlpha);

		mAlpha += mDeltaAlpha;
		if (mDeltaAlpha > 0)
		{
			if (mAlpha > 255)
			{
				mAlpha = 255;
				mDeltaAlpha = 0;

				if (!mKeepAfterFade)
				{
					removeFromScene();
				}
			}
		}
		else
		{
			if (mAlpha < 0)
			{
				mAlpha = 0;
				mDeltaAlpha = 0;

				if (!mKeepAfterFade)
				{
					removeFromScene();
				}
			}
		}
	}
}

void Overlay::setEffect(EffectType effect)
{
	mEffect = effect;
}
