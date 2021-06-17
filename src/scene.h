#ifndef SCENE_H
#define SCENE_H

#include <vector>//
#include <SDL.h>

#include "l_texture.h"
//feel free to add class headers if you need to use mCamera
#include "gameplay_map.h"//for friending
#include "overlay.h"//for friending
#include "sprite.h"//for friending
#include "player.h"//for friending

//TODO: fix multiple key down
//TODO: F11 can't be pressed when four arrow key down?


extern const int WINDOWED_WIDTH;
extern const int WINDOWED_HEIGHT;

class SceneObject;

class Scene
{
private:
	std::vector<SceneObject*> mObjects;//weak ptr
	LTexture mLTexture;

	SDL_Rect mCamera;
	float mSampleScale;
	bool mFullScreen;

public:
	Scene();

	void pushObject(SceneObject* object);
	bool removeObject(SceneObject* object);

//	void renderStart() const;
//	void renderEnd() const;
	void renderAll() const;

	void handleEvent(SDL_Event& e);//handle full screen
	void setCameraCenterPos(const int x, const int y);
	void smoothUpdate(const int x1, const int y1, const int x2, const int y2);

	//TODO: set border

private:
	void updateRenderScale();

	inline float sigmoid(const float x) const;
	inline float fabs(const float x) const;

	//feel free to add friend functions if you need to use mCamera
	friend void GameplayMap::render();
	friend void Overlay::render();
	friend void Sprite::render();
	friend void player::render();

	friend void checkCollision(player &opponent);
};

#endif // SCENE_H
