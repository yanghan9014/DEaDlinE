#ifndef OVERLAY_H
#define OVERLAY_H

#include <SDL.h>

#include "scene_object.h"


typedef unsigned char EffectType;

class Overlay : public SceneObject
{
public:
	enum : EffectType {
		NONE = 0, //00000000
		STRETCH = 1, //00000001
		STATIC = 2, //00000010
	};

private:
	float mDepth;
	int mOriginX;
	int mOriginY;

	int mAlpha;
	int mDeltaAlpha;

	EffectType mEffect;
	bool mKeepAfterFade;

public:
	Overlay(Scene* scene, const char* filepath, EffectType effect = NONE,
			const float depth = 1.0f, const int originX = 0, const int originY = 0);

	virtual void render() override;

	void setOriginX(const int x) { mOriginX = x; }
	void setOriginY(const int y) { mOriginX = y; }

	void setColor(const Uint8 red, const Uint8 green, const Uint8 blue);
	void setBlendMode(SDL_BlendMode blending = SDL_BLENDMODE_BLEND);
	void setAlpha(const Uint8 alpha);

	void setFade(const unsigned char start_alpha, const char delta_alpha, bool keep = false);
	bool isFading() const { return mDeltaAlpha != 0; }

	void setEffect(EffectType effect);

private:
	void fade();
};

#endif // OVERLAY_H
