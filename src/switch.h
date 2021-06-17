#ifndef SWITCH_H
#define SWITCH_H

#include"screen.h"


class loading_closing_screen : public screen
{
    public:

//        loading_closing_screen();
//
//        ~loading_closing_screen();

        void render();

        bool loadFromFile(const char* pathbackground ,const char* word  ,SDL_Color textColor );

        void setAlpha(uint8_t alpha);

};

#endif // SWITCH_H
