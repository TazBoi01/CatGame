#pragma once

#include <SDL.h>
#include <vector>
#include "GameObject.h"
#include "CollisionReferee.h"
#include "GameStateMachine.h"
#include "ImageHandler.h"
#include "SoundGameMusic.h"
#include "SoundReferee.h"

class SDLGame
{
private:
	// Pointer to the application window managed by SDL.
	SDL_Window* m_pWindow;

	// Pointer to the renderer that can be used to draw output.
	SDL_Renderer* m_pRenderer;

	// Referee to manage active colliders
	CollisionReferee m_collisionReferee;

	// State machine to manage scenes
	GameStateMachine* m_pStateMachine;

	// Referee to manage game sounds
	SoundReferee* m_pSoundReferee;

	// Game music
	SoundGameMusic* m_pSoundGameMusic;

	// Image Handler
	ImageHandler* m_pImageHandler;

	// Bool for whether to quit on next frame
	bool m_doQuit;

public:
	// Constructor and destructor
	SDLGame();
	~SDLGame();

	// Attempts to initialize SDL aspects of game. Returns an error code (0 means success).
	int InitSDL(GameStateMachine* pStateMachine);

	// Runs the game loop. Won't exit until a quit event has been detected.
	void RunGameLoop();

	// Deinitializes SDL aspects of game
	void DeinitSDL();

	// Quit game
	void Quit();

	// Gets games renderer
	SDL_Renderer* GetRenderer() { return m_pRenderer; }

	// Get games collision referee
	CollisionReferee* GetReferee() { return &m_collisionReferee; }

	// Get sound referee
	SoundReferee* GetSoundReferee() { return m_pSoundReferee; }

	// Get image handler
	ImageHandler* GetImageHandler() { return m_pImageHandler; }

private:
	// Processes game events, bool for if exit or not
	bool ProcessEvents();

	// Updates the state of the game simulation based on the passage of time and any events that were detected.
	void UpdateGameState(double deltaTime);

	// Draws this programs output
	void DisplayOutput();

};

