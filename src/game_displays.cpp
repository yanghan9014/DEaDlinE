#include "game_displays.h"
#include "player.h"


const int HP_FILL_WIDTH = 300;
const int HP_FILL_HEIGHT = 35;
const int DIGIT_WIDTH = 30;
const int DIGIT_HEIGHT = 60;

GameDisplays::GameDisplays(Scene* scene)
	:	mHpFill(scene, "overlays/hp_fill.png"),
		mHpBar(scene, "overlays/hp_bar.png", Overlay::STATIC, 1.0f, 0, 0),
		mDigits(scene, "overlays/digits.png"),
		mFailIcon(scene, "overlays/fail_icon.png")
{
	//hpFill
	SDL_Rect hpClipQuads[2] = {
		{ HP_FILL_WIDTH, 0, HP_FILL_WIDTH, HP_FILL_HEIGHT },
		{ 0, HP_FILL_HEIGHT, HP_FILL_WIDTH, HP_FILL_HEIGHT }
	};
	mHpFill.setClipQuads(2, hpClipQuads);

	int hpCurrentClips[2] = { 0, 1 };
	SDL_Rect hpDestQuads[2] = {
		{ 146, 72, HP_FILL_WIDTH, HP_FILL_HEIGHT },
		{ 833, 72, HP_FILL_WIDTH, HP_FILL_HEIGHT }
	};
	mHpFill.setDestQuads(2, hpCurrentClips, hpDestQuads);

	//Digits (for clock)
	SDL_Rect digitClipQuads[10] = {
		{ 0 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT },
		{ 1 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT },
		{ 2 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT },
		{ 3 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT },
		{ 4 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT },
		{ 5 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT },
		{ 6 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT },
		{ 7 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT },
		{ 8 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT },
		{ 9 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT },
	};
	mDigits.setClipQuads(10, digitClipQuads);

	int digitCurrentClips[2] = { 0, 0 };
	SDL_Rect digitDestQuads[2] = {
		{ 638 - DIGIT_WIDTH, 70, DIGIT_WIDTH, DIGIT_HEIGHT },
		{ 638, 70, DIGIT_WIDTH, DIGIT_HEIGHT }
	};
	mDigits.setDestQuads(2, digitCurrentClips, digitDestQuads);

	//failIcon
	SDL_Rect iconClipQuads[2] = {
		{ 0, 0, 40, 40 },
		{ 40, 0, 40, 40 }
	};
	mFailIcon.setClipQuads(2, iconClipQuads);

	int iconCurrentClips[4] = { 0, 0, 0, 0 };
	SDL_Rect iconDestQuads[4] = {
		{ 418, 127, 40, 40 },
		{ 466, 127, 40, 40 },
		{ 768, 127, 40, 40 },
		{ 816, 127, 40, 40 }
	};
	mFailIcon.setDestQuads(4, iconCurrentClips, iconDestQuads);


	mFailCount1 = 0;
	mFailCount2 = 0;

	mYouFailed.loadWav("audio/you_failed_2.wav");
	mDeath = false;
	mTransition = false;
}

void GameDisplays::start(player* player1, player* player2, const int time_seconds)
{
	if (player1)
		mPlayer1 = player1;

	if (player2)
		mPlayer2 = player2;

	mDeath = false;
	mStartTimeSeconds = time_seconds;
	mTimer.restart();
}
void GameDisplays::update()
{
	if (mDeath)
	{
		if (finished())
		{
			if (mTimer.getElapsed() > 4000)
				mTransition = false;
		}
		else
		{
			if (mTimer.getElapsed() > 3000)
					mTransition = false;
		}
		return;
	}


	int x1 = HP_FILL_WIDTH * (1.0f * (mPlayer1->get_cur_health()) / mPlayer1->get_full_health());
	if (x1 <= 0)
	{
		x1 = 0;
		mDeath = true;
		mTransition = true;
	}
	int x2 = HP_FILL_WIDTH * (1.0f * (mPlayer2->get_full_health() - mPlayer2->get_cur_health()) / mPlayer2->get_full_health());
	if (x2 >= HP_FILL_WIDTH)
	{
		x2 = HP_FILL_WIDTH;
		mDeath = true;
		mTransition = true;
	}


	SDL_Rect hpClipQuads[2] = {
		{ x1, 0, HP_FILL_WIDTH, HP_FILL_HEIGHT },
		{ x2, HP_FILL_HEIGHT, HP_FILL_WIDTH, HP_FILL_HEIGHT }
	};
	mHpFill.setClipQuads(2, hpClipQuads);

	int secondsLeft = mStartTimeSeconds - mTimer.getElapsed() / 1000;
	if (secondsLeft < 0) secondsLeft = 0;
	int digitCurrentClips[2] = { secondsLeft / 10, secondsLeft % 10 };
	mDigits.setDestQuads(2, digitCurrentClips, nullptr);

	if (mDeath)
	{
		recordScore();
		mTimer.restart();
		mYouFailed.play();
	}
}
void GameDisplays::recordScore()
{
	if (mPlayer1->get_cur_health() <= 0)
		mFailCount1++;

	if (mPlayer2->get_cur_health() <= 0)
		mFailCount2++;

	//printf("%d : %d\n", mFailCount1, mFailCount2);
	int iconCurrentClips[4] = { ( mFailCount1 > 0 ? 1 : 0),
								( mFailCount1 > 1 ? 1 : 0),
								( mFailCount2 > 0 ? 1 : 0),
								( mFailCount2 > 1 ? 1 : 0) };
	mFailIcon.setDestQuads(4, iconCurrentClips, nullptr);
}
bool GameDisplays::finished() const
{
	return (mFailCount1 > 1 || mFailCount2 > 1);
}
