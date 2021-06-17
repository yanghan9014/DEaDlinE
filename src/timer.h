#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>


#define TOLERENCE 10000

class Timer
{
private:
	Uint32 mStartTime;//in milliseconds
	int mBeatLength1000;//in microseconds

public:
	Timer()
	{
		restart();
	}
	Uint32 getElapsed()
	{
		return SDL_GetTicks() - mStartTime;
	}
	void restart()
	{
		mStartTime = SDL_GetTicks();
	}
	void restartBpm(const float bpm)
	{
		mBeatLength1000 = (int)(60 / bpm * 1000000);
		restart();
	}
	bool getBeat()
	{
		int delta = (getElapsed() * 1000) % mBeatLength1000;
		return (delta < TOLERENCE || delta > mBeatLength1000 - TOLERENCE);
	}
	void offset(const int ms)
	{
		mStartTime += ms;
	}
};


#endif // TIMER_H
