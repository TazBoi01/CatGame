#pragma once
#include "CatGameStateMachine.h"
#include "Enemy.h"
#include "GameState.h"
#include "Player.h"
#include "SDLGame.h"
#include <SDL_ttf.h>
#include <vector>

class GameObject;
struct Vector2;

class StateGameplay :
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

	// Enemy game object
	Enemy* m_pEnemy;

	// Number of players catnips last update
	size_t m_catnipNumber;

    // References to game objects in scene
    std::vector<GameObject*> m_gameObjects;

	// Gameplay seed
	unsigned int m_seed;

public:
	// Constructor/Destructor
	StateGameplay(CatGameStateMachine* pOwner);
	~StateGameplay();

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

