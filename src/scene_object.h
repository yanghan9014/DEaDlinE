#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <SDL.h>

#include "l_texture.h"


class Scene;

class SceneObject//just a template class
{
protected:
	LTexture mLTexture;
	SDL_Rect* mTextureClips;
	int mClipNum;

	Scene* mSubscribedScene;
	bool mNoScale;

protected:
	SceneObject(Scene* scene = nullptr);//somewhere someone used new[], which can't take init value(= =)
	virtual ~SceneObject();

public:
	virtual void render() = 0;
	bool noScale() const { return mNoScale; }

	void pushToSceneAgain();
	void removeFromScene();

	void setScene(Scene* scene);
};

#endif // SCENE_OBJECT_H
