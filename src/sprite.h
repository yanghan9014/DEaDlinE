#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <vector>

#include "scene_object.h"


class Sprite : public SceneObject
{
private:
	//SceneObject::mClipNum deprecated?
	std::vector<SDL_Rect> mClipQuads;

	std::vector<int> mCurrentClips;
	std::vector<SDL_Rect> mDestQuads;

public:
	Sprite( Scene* scene, const char* filepath, bool is_static = true);

	void setClipQuads(const int quad_count, const SDL_Rect* quads);
	void setDestQuads(const int quad_count, const int* current_clips, const SDL_Rect* quads);

	virtual void render() override;
};


#endif // SPRITE_H
