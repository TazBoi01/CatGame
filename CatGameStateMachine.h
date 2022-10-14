#pragma once
#include "GameStateMachine.h"
#include "ImageHandler.h"
#include "SaveData.h"
#include <SDL_ttf.h>

class GameState;
class SDLGame;
struct SDL_Renderer;

class CatGameStateMachine :
    public GameStateMachine
{
public:
	enum class SceneName
	{
		MainMenu,
		NewGame,
		Load,
		Gameplay,
		Boss,
		Win,
		Lose
	};

private:
	// SDLGame object that owns this machine
	SDLGame* m_pOwner;

	// Currently active scene
	GameState* m_pCurrentState;

	// State to enter next frame
	GameState* m_pNextState;

	// UI Font
	TTF_Font* m_pFont;

	// Game state save data
	SaveData m_saveData;

	// Current save index
	size_t m_index;

public:
	// Constructor/Destructor
	CatGameStateMachine(SDLGame* pOwner);
	~CatGameStateMachine();

	// Initializes state machine, returns false if error
	virtual bool Init() override;

	// Updates the current scene
	virtual void UpdateCurrentState(double deltaTime) override;

	// Renders the current scene
	virtual void RenderCurrentState(SDL_Renderer* pRenderer) override;

	// Handles current event, returns true if quit
	virtual bool HandleEvent(SDL_Event* pEvent) override;

	// Exits current state, enters new state
	virtual void ChangeState(GameState* pNewState) override;

	// Load the given scene on next frame
	void LoadScene(SceneName scene);

	// Cleans up state machine, returns false if error
	virtual bool Cleanup() override;

	// Gets the SDLGame object that is running this machine
	SDLGame* GetGame() { return m_pOwner; }

	// Gets a pointer to loaded save data
	SaveData* GetSave() { return &m_saveData; }

	// Gets save index
	size_t GetIndex() { return m_index; }

	// Sets current save index
	void SetIndex(size_t index) { m_index = index; }
};

