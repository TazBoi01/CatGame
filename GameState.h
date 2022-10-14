#pragma once

struct SDL_Renderer;
union SDL_Event;

class GameState
{
public:
	// Destructor
	virtual ~GameState() {}

	// Initializes this scene
	virtual void Enter() {}

	// Updates this scene
	virtual void Update(double) {}

	// Renders this scene
	virtual void Render(SDL_Renderer* pRenderer) = 0;

	// Handles given event, return true if quit
	virtual bool HandleEvent(SDL_Event* pEvent) = 0;

	// Clean up and exit state
	virtual void Exit() {}
};

