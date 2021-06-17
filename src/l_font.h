#ifndef LFONT_H
#define LFONT_H


class LFont
{
	public:
		LFont();

	protected:

	private:
};

#endif // LFONT_H

////testing font rendering
//gFont = TTF_OpenFont("fonts/msjh.ttc", 100);
//
//LTexture gTextTexture1;
//LTexture gTextTexture2;
//LTexture gTextTexture3;
//if( gFont == nullptr )
//{
//	printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
//	//success = false;
//}
//else
//{
//	//Render text
//	Uint16 texttest[] = { 0x7bc4, 0x4f8b, 0x6587, 0x5b57, 0 };//can't change line
//
//	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
//	//SDL_Color textColor = { 10, 5, 0 };
//	if( !gTextTexture1.loadFromRenderedText( texttest, textColor ) )
//	{
//		printf( "Failed to render text texture!\n" );
//		//success = false;
//	}
//
//	textColor = { 0x70, 0x70, 0x50 };
//	if( !gTextTexture2.loadFromRenderedText( texttest, textColor ) )
//	{
//		printf( "Failed to render text texture!\n" );
//		//success = false;
//	}
//	TTF_SetFontOutline(gFont, 1);
//	if( !gTextTexture3.loadFromRenderedText( texttest, textColor ) )
//	{
//		printf( "Failed to render text texture!\n" );
//		//success = false;
//	}
//}
////
