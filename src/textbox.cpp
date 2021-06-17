#include "textbox.h"

TextBox::TextBox(Scene* scene, const char* text, const int originX, const int originY)
	: SceneObject(scene)
{
	SDL_Color color = {20, 0, 5, 0xFF};
	mLTexture.loadFromRenderedText(text, color);

	mAlpha = 0;
	mDeltaAlpha = 0;

	mOriginX = originX;
	mOriginY = originY;

	mBlink = false;
}

void TextBox::render()
{
	fade();
	if (mSubscribedScene)
	{
		mLTexture.render(mOriginX, mOriginY);
	}
}

void TextBox::setFade(const unsigned char start_alpha, const char delta_alpha, bool blink)
{
	mLTexture.setBlendMode();

	mAlpha = start_alpha;
	mDeltaAlpha = delta_alpha;

	mBlink = blink;
}
void TextBox::fade()
{
	if (mDeltaAlpha)
	{
		mLTexture.setAlpha(mAlpha);

		mAlpha += mDeltaAlpha;
		if (mDeltaAlpha > 0)
		{
			if (mAlpha > 255)
			{
				mAlpha = 255;
				mDeltaAlpha = (mBlink ? (-1 * mDeltaAlpha) : 0);
			}
		}
		else
		{
			if (mAlpha < 0)
			{
				mAlpha = 0;
				mDeltaAlpha = (mBlink ? (-1 * mDeltaAlpha) : 0);
			}
		}
	}
}
