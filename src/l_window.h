#ifndef LWINDOW_H
#define LWINDOW_H

#include <sstream>
#include <SDL.h>

#include "timer.h"


class LWindow
{
private:
	Timer mTimer;

	//Window data
	SDL_Window* mWindow;

	//FullScreen window dimensions
	SDL_Rect mFullScreenRect;
	float mFullScreenScale;

	std::stringstream mCaption;
	float mFps;

	bool mMouseFocus;
	bool mKeyboardFocus;

	bool mFullScreen;
	bool mMinimized;
	bool mDebugScreen;
	bool mDoScreenshot;//

	bool mF11_down;
	bool mF3_down;
	bool mF2_down;

public:
	//Intializes internals
	LWindow();
	//Creates window
	bool init();
	//Creates renderer from internal window
	SDL_Renderer* createRenderer() const;
	//Deallocates internals
	void free();

	//Handles window events
	void handleEvent( SDL_Event& e );

	SDL_Rect getFullScreenRect() const { return mFullScreenRect; }
	float getFullScreenScale() const { return mFullScreenScale; }

	//Window focii
	bool hasMouseFocus() const { return mMouseFocus; }
	bool hasKeyboardFocus() const { return mKeyboardFocus; }
	bool isMinimized() const { return mMinimized; }
	bool isFullScreen() const { return mFullScreen; }

	bool isDebugScreen() const { return mDebugScreen; }
	void updateFps();

	bool shouldDoScreenshot() const { return mDoScreenshot; }
	void doneScreenshot() { mDoScreenshot = false; }

private:
	void updateCaption();
};

#endif // LWINDOW_H
