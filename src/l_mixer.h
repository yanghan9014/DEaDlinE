#ifndef LMIXER_H
#define LMIXER_H

#include <SDL.h>
#include <SDL_mixer.h>


class LMixer
{
private:
	Mix_Music *mMusic;//in addition to managing Mixer, will own the music as well
	int mMusicVolume;
	int mChannelVolume;
	bool mIsMuted;

public:
	LMixer();
	~LMixer();

	bool init();
	void free();

	void loadMus(const char* filepath);

	void handleEvent(SDL_Event& e);

	bool isPlaying();
	bool isMuted();
	void play(const int loop_count = -1);//infinite loops
	void pause();
	void resume();
	void halt();

	void setVolume(const int music_volume, const int channel_volume);
	void toggleMute();

	void fadeIn(const int loops = -1, const int ms = 5000);
	void fadeOut(const int ms = 250);
};

#endif // LMIXER_H
