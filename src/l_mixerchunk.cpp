#include <stdio.h>

#include "l_mixer.h"
#include "l_mixerchunk.h"


extern LMixer gMixer;

LMixerChunk::LMixerChunk()
{
	mChunk = nullptr;
}
LMixerChunk::~LMixerChunk()
{
	if (mChunk != nullptr)
	{
		free();
	}
}

void LMixerChunk::loadWav(const char* filepath)
{
	//printf( "[LMixerChunk::loadWav] loading from %s\n", filepath );
	mChunk = Mix_LoadWAV(filepath);
	if (mChunk == nullptr)
    {
        printf( "[LMixerChunk::loadWav] Failed to load from %s! SDL_mixer Error: %s\n", filepath, Mix_GetError() );
    }

    gMixer.setVolume(-1, -1);
}
void LMixerChunk::free()
{
    Mix_FreeChunk( mChunk );
	mChunk = nullptr;

	//printf( "[LMixerChunk::free] freed chunk\n");
}

void LMixerChunk::play(int loops)
{
	Mix_PlayChannel( -1, mChunk, loops );
}
