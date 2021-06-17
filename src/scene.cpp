#include <SDL_image.h>//

#include "l_renderer.h"
#include "l_window.h"
#include "scene.h"
#include "scene_object.h"


#define PLAYER_DEFAULT_DISTANCE 500
const int PLAYER_SIZE_HALF = 30;//TODO: get data from player sprite

extern LRenderer gRenderer;
extern LWindow gWindow;

Scene::Scene()
{
	mCamera = {0, 0, WINDOWED_WIDTH, WINDOWED_HEIGHT};
	mSampleScale = 1.0f;
	mFullScreen = gWindow.isFullScreen();
	updateRenderScale();

	mLTexture.createBlank(WINDOWED_WIDTH * 4, WINDOWED_HEIGHT * 4, SDL_TEXTUREACCESS_TARGET);
	mLTexture.setBlendMode();
}

void Scene::pushObject(SceneObject* object)
{
    mObjects.push_back(object);
}
bool Scene::removeObject(SceneObject* object)
{
	for (int i = 0; i < mObjects.size(); i++)
	{
		if (mObjects[i] == object)
		{
			mObjects.erase(mObjects.cbegin() + i);
			return true;
		}
	}
	return false;
}

//void Scene::renderStart() const
//{
//
//}
//void Scene::renderEnd() const
//{
//
//}
void Scene::renderAll() const
{
	mLTexture.setAsRenderTarget();//render to scene texture
	gRenderer.clear();

	for (auto i : mObjects)
	{
		if (!i->noScale())//renderer to scene texture, to be scaled
			i->render();
	}

	mLTexture.resetRenderTarget();//render to window

	if (gWindow.shouldDoScreenshot())//irrelevant
	{
		SDL_Rect clip = { 0, 0, mCamera.w, mCamera.h };
		SDL_Rect destQuad = {0, 0, WINDOWED_WIDTH, WINDOWED_HEIGHT};
		mLTexture.renderDest( &clip, &destQuad );

		SDL_Surface *shot = SDL_CreateRGBSurface(	0, WINDOWED_WIDTH, WINDOWED_HEIGHT,
													32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF );

		gRenderer.renderReadPixels(shot, &destQuad);
		if (IMG_SavePNG(shot, "screenshots/test.png") != 0)
		{
			printf("[Scene::renderEnd] taking screenshot failed! perhaps folder \"screenshot/\" is missing?\n");
		}
		else
		{
			printf("[Scene::renderEnd] screenshot taken(overwritten)\n\n");//TODO: time stamp
		}
		SDL_FreeSurface(shot);

		gWindow.doneScreenshot();
	}//

	if (mFullScreen)
	{
		SDL_Rect clip = { 0, 0, mCamera.w, mCamera.h };//from scene rect
		SDL_Rect destQuad = gWindow.getFullScreenRect();//to fullscreen rect
		mLTexture.renderDest( &clip, &destQuad );
	}
	else
	{
		SDL_Rect clip = { 0, 0, mCamera.w, mCamera.h };
		SDL_Rect destQuad = {0, 0, WINDOWED_WIDTH, WINDOWED_HEIGHT};//to window rect
		mLTexture.renderDest( &clip, &destQuad );
	}

	for (auto i : mObjects)
	{
		if (i->noScale())//rendering directly to window, will be on top of all scaled layer
			i->render();
	}

	gRenderer.present();//done
}

void Scene::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11)
	{
		mFullScreen = gWindow.isFullScreen();
		updateRenderScale();
	}
}
void Scene::setCameraCenterPos(const int x, const int y)
{
	mCamera.x = x - (mCamera.w / 2);
	mCamera.y = y - (mCamera.h / 2);
}
void Scene::smoothUpdate(const int x1, const int y1, const int x2, const int y2)
{
	//in map coord
	float avgX = ((x1 + x2) / 2 + PLAYER_SIZE_HALF);
	float avgY = ((y1 + y2) / 2 + PLAYER_SIZE_HALF);

	float newScale = 1 + 0.5f * sigmoid((fabs(x2 - x1) - 2 * PLAYER_DEFAULT_DISTANCE) / PLAYER_DEFAULT_DISTANCE );


	//intermediate var for smooth change
	static int newCameraX = mCamera.x;
	static int newCameraY = mCamera.y;

	//higher value means slower respond speed
	static const int responseRcp = 8;

	//( new coord: ( half of scaled WINDOWED_WIDTH ) left of avgX ) will take up 1 / responseRcp of old coord
	newCameraX = (int)(( (avgX - WINDOWED_WIDTH  * newScale / 2) + ((responseRcp - 1) * mCamera.x) ) / responseRcp);
	newCameraY = (int)(( (avgY - WINDOWED_HEIGHT * newScale / 2) + ((responseRcp - 1) * mCamera.y) ) / responseRcp);

	mSampleScale = (newScale + (responseRcp - 1) * mSampleScale) / responseRcp;

	mCamera.x = newCameraX;
	mCamera.y = newCameraY;

	updateRenderScale();
}
inline float Scene::sigmoid(const float x) const
{
	return (x / (1 + fabs(x)));
}
inline float Scene::fabs(const float x) const
{
	return (x > 0 ? x : (x * -1.0f));
}

void Scene::updateRenderScale()
{
	mCamera.w = (int)(WINDOWED_WIDTH * mSampleScale);
	mCamera.h = (int)(WINDOWED_HEIGHT * mSampleScale);
}

