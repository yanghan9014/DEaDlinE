#include <stdio.h>

#include "scene.h"
#include "scene_object.h"


SceneObject::SceneObject(Scene* scene)
{
	mTextureClips = nullptr;
	mClipNum = 0;

	mSubscribedScene = scene;
	if (scene)
		scene->pushObject(this);//auto push once

	mNoScale = false;
}
SceneObject::~SceneObject()
{
	mSubscribedScene->removeObject(this);

	delete[] mTextureClips;
	//printf("[SceneObject::~SceneObject] delete mTextureClips\n");//TODO: logging
}

void SceneObject::pushToSceneAgain()//manually add to scene if to be rendered as multiple layers
{
	mSubscribedScene->pushObject(this);
}
void SceneObject::removeFromScene()
{
	mSubscribedScene->removeObject(this);
}

void SceneObject::setScene(Scene* scene)
{
	if (scene)
	{
		mSubscribedScene = scene;
		scene->pushObject(this);//auto push once
	}
}
