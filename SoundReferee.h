#pragma once
#include <string>
#include <map>
#include "SDL_mixer.h"

class SoundReferee
{
private:
	// Sound map containing pointer to sounds
	std::map<std::string, Mix_Chunk*> m_sounds;
	// Music map containing pointer to music
	std::map<std::string, Mix_Music*> m_music;
public:
	// Constructor/Destructor
	SoundReferee();
	~SoundReferee();

	// Add sound to sound map
	void AddSound(std::string name, const char* path);

	// Add music to music map
	void AddMusic(std::string name, const char* path);

	// Play music, loop - (0 none, 1 loop once, -1 loop infinite)
	int PlaySound(std::string name, int channel, int loop);

	// Getter to return sound from map at index
	Mix_Chunk* GetSound(std::string soundName) { return m_sounds[soundName]; }

	// Getter to return music from map at index
	Mix_Music* GetMusic(std::string musicName) { return m_music[musicName]; }

};

