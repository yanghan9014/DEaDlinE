#include"screen.h"

screen::~screen()
{
    gBackgroundTexture.free();
    gTextTexture.free();
}

//void screen::loadBackground(const char* pathbackground)
//{
//    gBackgroundTexture.loadFromFile( pathbackground );
//}

//since we can not preset the position of text , so I do this
void screen::rendertext(int x, int y, const char* word , SDL_Color textColor )
{
    gTextTexture.render(x, y);
    gTextTexture.loadFromRenderedText( word, textColor);
}

