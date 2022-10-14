#pragma once
#include "SoundReferee.h"

class SoundGameMusic
{
private:
	// Pointer to SoundLoader
	SoundReferee* m_pSoundLoader;
	// Music tracks
	Mix_Music* m_pTrack1;
	Mix_Music* m_pTrack2;

	// Current track
	Mix_Music* m_pCurrentTrack;
public:

	// Constructor/Destructor
	SoundGameMusic(SoundReferee* pSoundReferee);
	~SoundGameMusic();

	// Plays loaded music in a loop
	void PlayMusic();

	// Stops the music
	void HaltMusic();

	// Updates current track
	void UpdateMusic();

};

