#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SDL.h>


class select_screen;


class GameState
{
public:
	enum Gs : unsigned char {
		INIT, INTRO, START, MENU,
		PLAY, EXIT, CLOSE//, ERROR
	};
	enum SubGs : unsigned char {
		NONE, MAIN, MAP, CHARACTER, TUTORIAL, SETTINGS,
		READY, FIGHT, REST, FINISH
	};

private:
	Gs mGameState;
	SubGs mSubGameState;

	select_screen* mSelScreen;//weak ptr

public:
	GameState();

	void operator= (Gs rhs) { mGameState = rhs; }//no cascading
	bool operator==(Gs rhs) const { return (mGameState == rhs); }
	bool operator!=(Gs rhs) const { return (mGameState != rhs); }

	void operator= (SubGs rhs) { mSubGameState = rhs; }//no cascading
	bool operator==(SubGs rhs) const { return (mSubGameState == rhs); }
	bool operator!=(SubGs rhs) const { return (mSubGameState != rhs); }

	void handleEvent(SDL_Event& e);
	bool subscribeToSelection(select_screen* selection);
};

#endif // GAMESTATE_H
