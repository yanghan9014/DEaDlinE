#ifndef LBUTTON_H_INCLUDED
#define LBUTTON_H_INCLUDED

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "scene_object.h"


enum LButtonSprite//TODO: encapsulate
{
	OUT = 0,
	IN = 1,
	DOWN = 2,
	TOTAL_SPRITE = 3
};

//The mouse button
class LButton : public SceneObject
{
    public:
        //Initializes internal variables
        LButton();//
        //LButton(Scene* scene);//

        //~LButton();

        //Sets top left position
        void setPosition( int x, int y );

        //Handles mouse event
        void handleEvent( SDL_Event* e );

        bool loadFromFile( const char* path );

        void setAlpha(uint8_t alpha);

        //Shows button sprite
        void render();

        //bool button_pressed;

        void change_sprite( LButtonSprite buttonstate);
        LButtonSprite get_sprite() const { return mCurrentSprite; }//

        void setSpriteClips(int height, int width);

    private:
        //Top left position
        SDL_Point mPosition;

        //LTexture gButtonSpriteSheetTexture;

        SDL_Rect gSpriteClips[ TOTAL_SPRITE ];
        //Currently used global sprite
        LButtonSprite mCurrentSprite;
};

#endif // LBUTTON_H_INCLUDED
