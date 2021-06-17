#include <iostream>

#include "l_texture.h"
#include "l_button.h"
//#include "screen.h"
#include "select.h"


extern LTexture gBackgroundTexture;
extern SDL_Rect gSpriteClips[ TOTAL_SPRITE ];

//Button constants
extern const int BUTTON_WIDTH = 280;
extern const int BUTTON_HEIGHT = 100;

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_RETURN,
	KEY_PRESS_SURFACE_TOTAL
};

LButton::LButton()
//LButton::LButton(Scene* scene)
//	:SceneObject(scene)
{
	mPosition.x = 0;
	mPosition.y = 0;
	//button_pressed = false;

	mCurrentSprite = OUT;
}

//LButton::~LButton()
//{
//	gButtonSpriteSheetTexture.free();
//}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

bool LButton::loadFromFile( const char* path )
{
    if(!mLTexture.loadFromFile( path ))return false;
    return true;
}

void LButton::setAlpha(uint8_t alpha)
{
    mLTexture.setAlpha(alpha);
}

void LButton::render()
{
	//Show current button sprite
	mLTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}

void LButton::change_sprite(LButtonSprite buttonstate)
{
    mCurrentSprite = buttonstate;
}

void LButton::setSpriteClips(int height, int width)
{
    for(int i = 0;i < TOTAL_SPRITE;++i)
    {
        gSpriteClips[ i ].x = 0;
        gSpriteClips[ i ].y = i*height;
        gSpriteClips[ i ].w = width;
        gSpriteClips[ i ].h = height;
    }
}
