#include "sprite.h"
#include "scene.h"
#include "l_window.h"


extern LWindow gWindow;

Sprite::Sprite( Scene* scene, const char* filepath, bool is_static)
	: SceneObject(scene)
{
	mNoScale = is_static;//all Overlay::STATIC for now
	mLTexture.loadFromFile(filepath);
}

void Sprite::setClipQuads(const int quad_count, const SDL_Rect* quads)
{
	if (quads)
	{
		mClipQuads.resize(quad_count);
		for (int i = 0; i < quad_count; i++)
		{
			mClipQuads[i] = quads[i];
		}
	}
}
void Sprite::setDestQuads(const int quad_count, const int* current_clips, const SDL_Rect* quads)
{
	if (current_clips)
	{
		mCurrentClips.resize(quad_count);
		for (int i = 0; i < quad_count; i++)
		{
			mCurrentClips[i] = current_clips[i];
		}
	}

	if (quads)
	{
		mDestQuads.resize(quad_count);
		for (int i = 0; i < quad_count; i++)
		{
			mDestQuads[i] = quads[i];
		}
	}
}

void Sprite::render()
{
	//all Overlay::STATIC for now
	if (mNoScale)
	{
		for (int i = 0; i < mDestQuads.size(); i++)
		{
			if (gWindow.isFullScreen())
			{
				SDL_Rect destQuad = {
					mDestQuads[i].x * gWindow.getFullScreenScale(),
					mDestQuads[i].y * gWindow.getFullScreenScale(),
					mDestQuads[i].w * gWindow.getFullScreenScale(),
					mDestQuads[i].h * gWindow.getFullScreenScale()
				};
				mLTexture.renderDest(&( mClipQuads[ mCurrentClips[i] ] ), &destQuad);
			}
			else
			{
				mLTexture.renderDest(&( mClipQuads[ mCurrentClips[i] ] ), &( mDestQuads[i]) );
			}
		}
	}
	else
	{
		//SDL_Rect& cam = mSubscribedScene->mCamera;
		for (int i = 0; i < mDestQuads.size(); i++)
		{
			mLTexture.render( mDestQuads[i].x - mSubscribedScene->mCamera.x,
							  mDestQuads[i].y - mSubscribedScene->mCamera.y,
							  &( mClipQuads[ mCurrentClips[i] ] ) );
		}
	}
}
