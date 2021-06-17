#include "l_window.h"
#include "l_renderer.h"

#include <iomanip>
#include <stdio.h>


#define FPS_SAMPLE_FRAME_COUNT 30

extern const int WINDOWED_WIDTH;
extern const int WINDOWED_HEIGHT;

extern LRenderer gRenderer;//extern or keep pointer?

LWindow::LWindow()
{
    //Initialize non-existant window
    mWindow = nullptr;

    mFps = 0.0f;
	mCaption << std::setprecision(1) << std::fixed;

    mMouseFocus = false;
    mKeyboardFocus = false;
    mFullScreen = false;
    mMinimized = false;
    mDebugScreen = false;//make a global bool?

	mF11_down = false;
	mF3_down = false;
	mF2_down = false;
}

bool LWindow::init()
{
    //Create window
    mWindow = SDL_CreateWindow( "SDL Tutorial",
								SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWED_WIDTH, WINDOWED_HEIGHT,
								SDL_WINDOW_SHOWN  );
    if( mWindow != nullptr )
    {
        mMouseFocus = true;
        mKeyboardFocus = true;

        int i = SDL_GetWindowDisplayIndex(mWindow);
		SDL_Rect r;
		SDL_GetDisplayBounds(i, &r);

		mFullScreenRect = {0, 0, r.w, r.h};

		//make part of screen black if aspect ratio doesn't match
		//fullScreen width is too long
		if (mFullScreenRect.w * WINDOWED_HEIGHT > mFullScreenRect.h * WINDOWED_WIDTH)
		{
			mFullScreenScale = (float)mFullScreenRect.h / WINDOWED_HEIGHT;
			mFullScreenRect.x = (mFullScreenRect.w - WINDOWED_WIDTH * mFullScreenScale) / 2;
			mFullScreenRect.w = WINDOWED_WIDTH * mFullScreenScale;
		}
		//fullScreen height is too long
		else
		{
			mFullScreenScale = (float)mFullScreenRect.w / WINDOWED_WIDTH;
			mFullScreenRect.y = (mFullScreenRect.h - WINDOWED_HEIGHT * mFullScreenScale) / 2;
			mFullScreenRect.h = WINDOWED_HEIGHT * mFullScreenScale;
		}
    }

    return mWindow != nullptr;
}

SDL_Renderer* LWindow::createRenderer() const
{
    return SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
								SDL_RENDERER_TARGETTEXTURE );
}

void LWindow::handleEvent( SDL_Event& e )
{
    //Window event occured
    if( e.type == SDL_WINDOWEVENT )
    {
        //Caption update flag
        bool toUpdateCaption = false;
		switch( e.window.event )
        {
            //Repaint on exposure
            case SDL_WINDOWEVENT_EXPOSED:
				gRenderer.present();
				break;

             //Mouse entered window
            case SDL_WINDOWEVENT_ENTER:
				mMouseFocus = true;
				toUpdateCaption = true;
				break;

            //Mouse left window
            case SDL_WINDOWEVENT_LEAVE:
				mMouseFocus = false;
				toUpdateCaption = true;
				break;

            //Window has keyboard focus
            case SDL_WINDOWEVENT_FOCUS_GAINED:
				mKeyboardFocus = true;
				toUpdateCaption = true;
				break;

            //Window lost keyboard focus
            case SDL_WINDOWEVENT_FOCUS_LOST:
				mKeyboardFocus = false;
				toUpdateCaption = true;
				break;

            //Window minimized
            case SDL_WINDOWEVENT_MINIMIZED:
				mMinimized = true;
				break;

            //Window maxized
            case SDL_WINDOWEVENT_MAXIMIZED:
				mMinimized = false;
				break;

            //Window restored
            case SDL_WINDOWEVENT_RESTORED:
				mMinimized = false;
				break;
        }
        //Update window caption with new data
        if( toUpdateCaption )
        {
            updateCaption();
        }
    }
    //Enter or exit full screen
    else if( e.type == SDL_KEYDOWN )
    {
    	switch (e.key.keysym.sym)
    	{
		case SDLK_F11:
			if (!mF11_down)
			{
				mF11_down = true;

				if (!mFullScreen)
				{
					SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				else
				{
					SDL_SetWindowFullscreen(mWindow, 0);
				}
				mFullScreen = !mFullScreen;
			}
			break;

		case SDLK_F3:
			if (!mF3_down)
			{
				mF3_down = true;
				mDebugScreen = !mDebugScreen;
				updateCaption();
			}
			break;
		case SDLK_F2:
			if (!mF2_down)
			{
				mF2_down = true;
				mDoScreenshot = true;
				printf("[LWindow::handleEvent] taking screenshot...\n");
			}
			break;
    	}
    }
    else if( e.type == SDL_KEYUP )
    {
    	switch (e.key.keysym.sym)
    	{
		case SDLK_F11:
			if (mF11_down)
			{
				mF11_down = false;
			}
			break;
		case SDLK_F3:
			if (mF3_down)
			{
				mF3_down = false;
			}
			break;
		case SDLK_F2:
			if (mF2_down)
			{
				mF2_down = false;
			}
			break;
    	}
    }
}
void LWindow::free()
{
	SDL_DestroyWindow( mWindow );
}
void LWindow::updateFps()
{
	static int frameCount = 0;

	frameCount++;
	if (frameCount == FPS_SAMPLE_FRAME_COUNT)
	{
		frameCount = 0;
		mFps = FPS_SAMPLE_FRAME_COUNT * 1000.0f / mTimer.getElapsed();
		mTimer.restart();

		updateCaption();
	}
}
void LWindow::updateCaption()
{
	mCaption.str("");
	//TODO: remove, dispose of unused "focus" flags
	//mCaption << mFps <<"fps MouseFocus:" << ( ( mMouseFocus ) ? "On" : "Off" ) << " KeyboardFocus:" << ( ( mKeyboardFocus ) ? "On" : "Off" );
	if (mDebugScreen)
	{
		mCaption << "[" << mFps << "fps] ";
	}
	mCaption << "DEadLinE - alpha 0.1";
	SDL_SetWindowTitle( mWindow, mCaption.str().c_str() );
}
