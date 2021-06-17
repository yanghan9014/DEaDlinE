#ifndef GAMEDISPLAYS_H
#define GAMEDISPLAYS_H

#include <SDL.h>

#include "overlay.h"
#include "sprite.h"
#include "timer.h"
#include "l_mixerchunk.h"


class player;

class GameDisplays
{
private:
	Sprite mHpFill;
	Overlay mHpBar;
	Sprite mDigits;
	Sprite mFailIcon;

	player* mPlayer1;
	player* mPlayer2;

	Timer mTimer;
	int mStartTimeSeconds;

	union {
		unsigned char c;
		struct {
			int mFailCount1 : 4;
			int mFailCount2 : 4;
		};
	};

	LMixerChunk mYouFailed;
	bool mDeath;
	bool mTransition;

public:
	GameDisplays(Scene* scene);

	void clockStart(const int duration = 60) { mTimer.restart(); }
	int getTimeMs() { return mTimer.getElapsed(); }

	void start(player* player1, player* player2, const int time_seconds = 60);
	void update();
	void recordScore();

	bool death() const { return mDeath; }
	bool doingTransition() const { return mTransition; }
	bool finished() const;
};

#endif // GAMEDISPLAYS_H
