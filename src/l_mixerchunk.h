#ifndef LMIXERCHUNK_H
#define LMIXERCHUNK_H

#include <SDL.h>
#include <SDL_mixer.h>


class LMixerChunk
{
private:
	Mix_Chunk* mChunk;

public:
	LMixerChunk();
	~LMixerChunk();

	void loadWav(const char* filepath);
	void free();

	void play(int loops = 0);

	//int getChannelsPlaying() const { return Mix_Playing(-1); }
};

#endif // LMIXERCHUNK_H
