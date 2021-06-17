#include <stdio.h>

#include "gamestate.h"
#include "select.h"


GameState::GameState()
{
	mGameState = INIT;
	mSubGameState = NONE;

	mSelScreen = nullptr;
}

void GameState::handleEvent(SDL_Event& e)
{
	if ( e.type == SDL_KEYDOWN )
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_ESCAPE:
				mGameState = CLOSE;
			break;

		case SDLK_RETURN:
			switch (mGameState)
			{
			case INTRO:
				mGameState = START;
				break;

			case START:
				mGameState = MENU;
				mSubGameState = MAIN;
				break;

			case MENU:
				switch (mSubGameState)
				{
				case MAIN:
					mSelScreen->down(0);
					switch (mSelScreen->getBitCounter().getbit())
					{
					case 0: mSubGameState = READY; mGameState = PLAY; break;
					case 1: mSubGameState = MAP; break;
					case 2: mSubGameState = CHARACTER; break;
					case 3: mSubGameState = TUTORIAL; break;
					//case 4: mSubGameState = SETTINGS; break;
					case 5: mSubGameState = NONE; mGameState = EXIT; break;
					case 6: mSubGameState = NONE; mGameState = START; break;
					}
					break;
				case MAP:
					mSelScreen->down(1);
					mSubGameState = MAIN;
					break;

				case CHARACTER:
					if (mSelScreen->isFirstSelection())
					{
						if (!mSelScreen->down(1))
							mSubGameState = MAIN;
					}
					else
					{
						mSelScreen->down(2);
						mSubGameState = MAIN;
					}
					break;
				case TUTORIAL:
					mSubGameState = MAIN;
					break;

				default: break;
				}
				break;

			default: break;
			}
			break;

		case SDLK_UP:
			switch (mSubGameState)
			{
			case MAIN:		!(mSelScreen->getBitCounter()); break;
			case CHARACTER:	++(mSelScreen->getBitCounter()); break;
			case MAP:		++(mSelScreen->getBitCounter()); break;

			default: break;
			}
			if (mSelScreen)
				mSelScreen->update();
			break;

		case SDLK_DOWN:
			switch (mSubGameState)
			{
			case MAIN:		~(mSelScreen->getBitCounter()); break;
			case CHARACTER:	--(mSelScreen->getBitCounter()); break;
			case MAP:		--(mSelScreen->getBitCounter()); break;

			default: break;
			}
			if (mSelScreen)
				mSelScreen->update();
			break;

		case SDLK_LEFT:
			switch (mSubGameState)
			{
			case MAIN:		if (mSelScreen->getBitCounter().getbit() == 6)
							{
								(!(mSelScreen->getBitCounter()));
							}
							break;
			case CHARACTER:	-(mSelScreen->getBitCounter()); break;
			case MAP:		-(mSelScreen->getBitCounter()); break;

			default: break;
			}
			if (mSelScreen)
				mSelScreen->update();
			break;

		case SDLK_RIGHT:
			switch (mSubGameState)
			{
			case MAIN:		if (mSelScreen->getBitCounter().getbit() == 5)
							{
								(--(mSelScreen->getBitCounter()));
							}
							break;
			case CHARACTER:	+(mSelScreen->getBitCounter()); break;
			case MAP:		+(mSelScreen->getBitCounter()); break;

			default: break;
			}
			if (mSelScreen)
				mSelScreen->update();
			break;
		}
	}
	else if( e.type == SDL_QUIT )
	{
		mGameState = CLOSE;
	}
}
bool GameState::subscribeToSelection(select_screen* selection)
{
	if (mSelScreen != selection)
	{
		mSelScreen = selection;
		return true;
	}
	return false;
}
