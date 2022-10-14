#pragma once

class GameState;
struct SDL_Renderer;
union SDL_Event;

class GameStateMachine
{
public:
	// Destructor
	virtual ~GameStateMachine() {}
	
	// Initializes state machine, returns false if error
	virtual bool Init() = 0;

	// Updates the current scene
	virtual void UpdateCurrentState(double deltaTime) = 0;

	// Renders the current scene
	virtual void RenderCurrentState(SDL_Renderer* pRenderer) = 0;

	// Handles current event, returns true if quit
	virtual bool HandleEvent(SDL_Event* pEvent) = 0;

	// Exits current state, enters new state
	virtual void ChangeState(GameState* pNewState) = 0;

	// Cleans up state machine, returns false if error
	virtual bool Cleanup() = 0;
};

