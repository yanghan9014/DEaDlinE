#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include "l_texture.h"
#include "l_button.h"


const int TOTAL_BUTTONS = 7;

class screen
{
    public :

        ~screen();

        virtual void render() = 0;
        void rendertext(int x, int y, const char* word, SDL_Color textColor );

    protected :

        LTexture gBackgroundTexture;

        LTexture gTextTexture;

};

#endif // SCREEN_H_INCLUDED
