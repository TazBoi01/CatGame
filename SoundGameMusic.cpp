#include "SoundGameMusic.h"

SoundGameMusic::SoundGameMusic(SoundReferee* pSoundReferee)
	: m_pCurrentTrack(nullptr)
	, m_pTrack1(nullptr)
	, m_pTrack2(nullptr)
	, m_pSoundLoader(pSoundReferee)
{
	m_pTrack1 = m_pSoundLoader->GetMusic("music1");
	m_pTrack2 = m_pSoundLoader->GetMusic("music2");
	m_pCurrentTrack = m_pTrack1;
}

SoundGameMusic::~SoundGameMusic()
{
}

///////////////////////////////
// Plays loaded music in a loop
void SoundGameMusic::PlayMusic()
{
	Mix_VolumeMusic(10);
	Mix_PlayMusic(m_pCurrentTrack, 0);
}

//////////////////
// Stops the music
void SoundGameMusic::HaltMusic()
{
	Mix_HaltMusic();
}

////////////////////////
// Updates current track
void SoundGameMusic::UpdateMusic()
{
	if (!Mix_PlayingMusic())
	{
		if (m_pCurrentTrack != nullptr)
		{
			if (m_pCurrentTrack == m_pTrack1)
				m_pCurrentTrack = m_pTrack2;
			else
				m_pCurrentTrack = m_pTrack1;
		}
		PlayMusic();
	}
}
