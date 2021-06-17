#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SDL.h>

#include "scene_object.h"


class TextBox : public SceneObject
{
private:
	int mOriginX;
	int mOriginY;

	int mAlpha;
	int mDeltaAlpha;

	bool mBlink;

public:
	TextBox(Scene* scene, const char* text, const int originX = 0, const int originY = 0);

	virtual void render() override;
	void setScene(Scene* scene) { SceneObject::setScene(scene); }

	void setOriginX(const int x) { mOriginX = x; }
	void setOriginY(const int y) { mOriginX = y; }

	void setFade(const unsigned char start_alpha, const char delta_alpha, bool blink = false);
	bool isFading() const { return mDeltaAlpha != 0; }

private:
	void fade();
};

#endif // TEXTBOX_H
