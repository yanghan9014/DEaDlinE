#include <stdio.h>

#include "l_mixer.h"


#define DEFAULT_MUSIC_VOLUME MIX_MAX_VOLUME / 8
#define DEFAULT_CHUNK_VOLUME MIX_MAX_VOLUME / 4


LMixer::LMixer()
{
	mMusic = nullptr;
	mMusicVolume = DEFAULT_MUSIC_VOLUME;
	mChannelVolume = DEFAULT_CHUNK_VOLUME;
	mIsMuted = false;

	setVolume(DEFAULT_MUSIC_VOLUME, DEFAULT_CHUNK_VOLUME);
}
LMixer::~LMixer()
{
	if (mMusic != nullptr)
	{
		free();
	}
}

bool LMixer::init()
{
	return ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == 0 );
	//Mix_AllocateChannels(8);//can specify number of channels allocated
}
void LMixer::free()
{
    Mix_FreeMusic( mMusic );
	mMusic = nullptr;

	//printf( "[LMixer::free] freed music\n");
}

void LMixer::loadMus(const char* filepath)
{
	if (mMusic != nullptr)
    {
        free();
    }

	//printf( "[LMixer::loadMus] loading from %s\n", filepath );
    mMusic = Mix_LoadMUS(filepath);
    if (mMusic == nullptr)
    {
        printf( "[LMixer::loadMus] Failed to load from %s! SDL_mixer Error: %s\n", filepath, Mix_GetError() );
    }

	setVolume(mMusicVolume, -1);
}

void LMixer::handleEvent(SDL_Event& e)//TODO: replace with graphical interface
{
	//empty
}
bool LMixer::isPlaying()
{
	return Mix_PlayingMusic();
}
bool LMixer::isMuted()
{
	return mIsMuted;
}

void LMixer::play(const int loop_count)
{
	Mix_PlayMusic( mMusic, loop_count );
}
void LMixer::pause()
{
	Mix_PauseMusic();
}
void LMixer::resume()
{
	Mix_ResumeMusic();
}
void LMixer::halt()
{
	Mix_HaltMusic();
}

void LMixer::setVolume(const int music_volume, const int channel_volume)
{
	Mix_VolumeMusic( music_volume >= 0 ? music_volume : mMusicVolume );
	Mix_Volume( -1, channel_volume >= 0 ? channel_volume : mChannelVolume );
}
void LMixer::toggleMute()
{
	mIsMuted = !mIsMuted;
	setVolume((int)(!mIsMuted) * mMusicVolume, (int)(!mIsMuted) * mChannelVolume);
}

void LMixer::fadeIn(const int loops, const int ms)
{
	Mix_FadeInMusic(mMusic, loops, ms);
}
void LMixer::fadeOut(const int ms)
{
	Mix_FadeOutMusic(ms);
}
