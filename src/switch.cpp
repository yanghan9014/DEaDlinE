#include "switch.h"

//loading_closing_screen::loading_closing_screen()
//{
//
//}
//
//loading_closing_screen::~loading_closing_screen()
//{
//
//}

void loading_closing_screen::render()
{
    gBackgroundTexture.render(0, 0);
//    gTextTexture.render();
}

bool loading_closing_screen::loadFromFile ( const char* pathbackground , const char* word ,SDL_Color textColor )
{
    printf("[switch]loadFromFile\n");
    if(!gBackgroundTexture.loadFromFile( pathbackground )) return false;
    else if(!gTextTexture.loadFromRenderedText(word, textColor)) return false;
    else return true;
}

void loading_closing_screen::setAlpha(uint8_t alpha)
{
    gBackgroundTexture.setAlpha(alpha);
    gTextTexture.setAlpha(alpha);
}
