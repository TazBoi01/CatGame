#include "SoundReferee.h"
#include <iostream>

SoundReferee::SoundReferee()
{
	AddMusic("music1", "assets/music1.mp3");
	AddMusic("music2", "assets/music2.mp3");
	AddSound("win", "assets/win.wav");
	AddSound("walk", "assets/walk.wav");
	AddSound("click", "assets/click.wav");
	AddSound("explosion", "assets/explosion.wav");
	AddSound("hit", "assets/hit.wav");
}

SoundReferee::~SoundReferee()
{
	// Free music and chunks
	for (auto iterator = m_music.begin(); iterator != m_music.end(); iterator++)
		Mix_FreeChunk(GetSound(iterator->first));
	for (auto iterator = m_sounds.begin(); iterator != m_sounds.end(); iterator++)
		Mix_FreeChunk(GetSound(iterator->first));
}

/////////////////////////
// Add sound to sound map
void SoundReferee::AddSound(std::string name, const char* path)
{
	// Load a sound with given path, place it in the map
	Mix_Chunk* pSound = Mix_LoadWAV(path);
	if (pSound == nullptr)
	{
		std::cout << "Failed to load sound. Error: " << Mix_GetError();
		return;
	}
	m_sounds[name] = pSound;
}

/////////////////////////
// Add music to music map
void SoundReferee::AddMusic(std::string name, const char* path)
{
	// Load music with given path, place it in the map
	Mix_Music* pMusic = Mix_LoadMUS(path);
	if (pMusic == nullptr)
	{
		std::cout << "Failed to load music. Error: " << Mix_GetError();
		return;
	}
	m_music[name] = pMusic;
}

/////////////////////////////////////////////////////////////
// Play music, loop - (0 none, 1 loop once, -1 loop infinite)
int SoundReferee::PlaySound(std::string name, int channel, int loop)
{
	channel = Mix_PlayChannel(-1, GetSound(name), loop);
	return channel;
}
