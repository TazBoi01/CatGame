#pragma once
#include "Boss.h"
#include "CatGameStateMachine.h"
#include "GameState.h"
#include "Player.h"
#include "SDLGame.h"
#include <SDL_ttf.h>
#include <vector>

class GameObject;

class StateBoss :
	public GameState
{
private:
	// State machine that is operating this state
	CatGameStateMachine* m_pOwner;

	// This games renderer
	SDL_Renderer* m_pRenderer;

	// This games collision referee
	CollisionReferee* m_pCollisionReferee;

	// This games sound referee
	SoundReferee* m_pSoundReferee;

	// Image Handler
	ImageHandler* m_pImageHandler;

	// Player game object
	Player* m_pPlayer;

	// Boss game object
	Boss* m_pBoss;

	// Number of players catnips last update
	size_t m_catnipNumber;

	// If catnip present on board
	bool m_catnipSpawned;

	// Seconds countdown for when boss defeated
	double m_seconds;

	// References to game objects in scene
	std::vector<GameObject*> m_gameObjects;

public:
	// Constructor/Destructor
	StateBoss(CatGameStateMachine* pOwner);
	~StateBoss();

	// Initializes this scene
	virtual void Enter() override;

	// Updates this scene
	virtual void Update(double deltaTime) override;

	// Renders this scene
	virtual void Render(SDL_Renderer* pRenderer) override;

	// Handles given event, return true if quit
	virtual bool HandleEvent(SDL_Event* pEvent) override;

	// Clean up and exit state
	virtual void Exit() override;

private:
	// Saves this game, returns if successful
	bool Save();
};
