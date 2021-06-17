#include "select.h"


const int TOTAL_BUTTONS = 7;//

select_screen::select_screen(Scene* scene)
	: SceneObject(scene)
{
	mActive = false;

	gButtons = nullptr;
    button_num = TOTAL_BUTTONS;

    selection = 0;
    selection2 = -1;
    mFirstSelection = true;

    gButtons = new LButton[button_num];
    if (scene)
	{
		for (int i = 0; i < button_num; i++)
		{
			gButtons[i].setScene(scene);
		}
	}

    gButtons[0].change_sprite(DOWN);

	mButtonMove.loadWav("audio/button_move.wav");
	mButtonDown.loadWav("audio/button_down.wav");
	mDown = false;
	mButtonSpriteChange = false;
}
void select_screen::setScene(Scene* scene)
{
	if (scene)
	{
		SceneObject::setScene(scene);
		for (int i = 0; i < button_num; i++)
		{
			gButtons[i].setScene(scene);
		}
	}
}
select_screen::~select_screen()//
{
    delete[] gButtons;
}

void select_screen::render()
{
	if (mSubscribedScene)
	{
		mLTexture.render(0, 0);
		for(int i = 0; i < button_num;i++)gButtons[i].render();
	}
}

void select_screen::setposition(int x0,int y0,int x1,int y1, int x2,int y2,int x3,int y3,int x4, int y4,int x5,int y5,int x6,int y6)
{
    gButtons[0].setPosition(x0 ,y0);
    gButtons[1].setPosition(x1 ,y1);
    gButtons[2].setPosition(x2 ,y2);
    gButtons[3].setPosition(x3 ,y3);
    gButtons[4].setPosition(x4 ,y4);
    gButtons[5].setPosition(x5 ,y5);
    gButtons[6].setPosition(x6 ,y6);
}
//TODO: deal with words
bool select_screen::loadFromFile( const char* path0 ,const char* path1 ,const char* path2 ,const char* path3 ,const char* path4 ,
                                  const char* path5 ,const char* path6 ,const char* pathbackground ,const char* word  ,SDL_Color textColor )
{
    if( !gButtons[0].loadFromFile( path0 )                  ||
        !gButtons[1].loadFromFile( path1 )                  ||
        !gButtons[2].loadFromFile( path2 )                  ||
        !gButtons[3].loadFromFile( path3 )                  ||
        !gButtons[4].loadFromFile( path4 )                  ||
        !gButtons[5].loadFromFile( path5 )                  ||
        !gButtons[6].loadFromFile( path6 )                  ||
        !mLTexture.loadFromFile( pathbackground )
        // || !gTextTexture.loadFromRenderedText(word, textColor)
        )  return false;

    return true;
}

// I need to black out the screen, and the button also need to black out simultaneously so I made this
void select_screen::setAlpha(uint8_t alpha)
{
    mLTexture.setAlpha(alpha);
    for(int i = 0;i < TOTAL_BUTTONS;i++)gButtons[i].setAlpha(alpha);
}

void select_screen::change_sprite( int bit, LButtonSprite buttonstate )
{
//	printf("%d is changed to ", bit);
//	switch (buttonstate)
//	{
//		case IN: printf("IN\n"); break;
//		case OUT: printf("OUT\n"); break;
//		case DOWN: printf("DOWN\n"); break;
//	}

    if (buttonstate == DOWN)
	{
		for (int i = 0; i < TOTAL_BUTTONS; i++)
		{
			if (i != selection && i != selection2)//
			{
				if ( gButtons[i].get_sprite() == DOWN )
					gButtons[i].change_sprite(OUT);
			}
		}
	}
    gButtons[bit].change_sprite(buttonstate);
}

void select_screen::setSpriteClips(int height, int width)
{
    for(int i = 0;i < TOTAL_BUTTONS;++i)
    {
        gButtons[i].setSpriteClips( height, width );
    }
}

int select_screen::getSelection(const int which) const
{
    if (which == 1)
		return selection;
	else
		return selection2;
}
void select_screen::setSelection(const int which, const int num)
{
	if (which == 1)
		selection = num;
	else
		selection2 = num;

    update();
}
void select_screen::update()
{
	mButtonSpriteChange = false;
	for (int i = 0; i < TOTAL_BUTTONS; i++)
	{
		if (i == selection || i == selection2)
		{
			gButtons[i].change_sprite(DOWN);
		}
		else if (i == mBitCounter.getbit())
		{
			if (gButtons[i].get_sprite() != IN)
				mButtonSpriteChange = true;

			gButtons[i].change_sprite(IN);
		}
		else
		{
			if (gButtons[i].get_sprite() != OUT)
				mButtonSpriteChange = true;

			gButtons[i].change_sprite(OUT);
		}
	}

	if (mActive)
	{
		if (!mDown && mButtonSpriteChange)
		{
			mButtonMove.play();
		}
	}
}
bool select_screen::down(int which)//return whether to stay at this menu
{
	if (mActive)
	{
		mButtonDown.play();
	}

	if (which == 0)//for MAIN, go to PLAY
		return false;
	if (mBitCounter.getbit() == 6)//go back
	{
		mBitCounter.reset();
		mFirstSelection = true;
		return false;//go back
	}
	else
	{
		mDown = true;
		setSelection(which, mBitCounter.getbit());
		mDown = false;

		return true;
	}
}


