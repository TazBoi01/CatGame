#pragma once
#include "AIStateMachine.h"
#include "AnimationComponent.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "HealthBar.h"
#include "RandomNumberGenerator.h"

class Boss :
    public GameObject
{
private:
	const float m_kMaxHealth = 1000;

	// Enemy health
	float m_health;

	// Movement speed.
	const double m_kSpeed = 100;

	// Health UI
	HealthBar m_healthBar;

	// Position horizontal
	float m_x;

	// Position vertical
	float m_y;

	// Current direction to move (-1 left, 1 right)
	float m_horizontalDirection;

	//(-1 up, 1 down)
	float m_verticalDirection;

	// Enemy current target
	GameObject* m_pTarget;

	// String for current animation playing
	std::string m_currentAnimation;

	// Bool for if currently playing animation
	bool m_playingAnimation;

	// Allocates an SDL_FRect for rendering.
	SDL_FRect m_transform;

	// Allocate animation component
	AnimationComponent m_animation;

	// Allocate collider component
	ColliderComponent m_collider;

	// Enemy AI state machine
	AIStateMachine m_stateMachine;

	// Pointer to renderer to spawn projectiles
	SDL_Renderer* m_pRenderer;

	// Pointer to collision referee to spawn projectiles
	CollisionReferee* m_pReferee;

	// Random Number Generator for deciding moves
	RandomNumberGenerator m_rng;

	// Seconds storage
	double m_seconds;

public:
	// Constructor/Destructor
	Boss(SDL_Renderer* pRenderer, SDL_Surface* pImageBoss, SDL_Surface* pImageBar, float x, float y, GameObject* pTarget, CollisionReferee* pReferee);
	~Boss();

	// Set player health
	void SetHealth(float health) { m_health = health; }

	// Get this objects current state
	AIStateMachine::StateName GetCurrentState() { return m_stateMachine.GetCurrentStateName(); }

	// Set this objects current state
	void SetCurrentState(AIStateMachine::StateName newState) { m_stateMachine.ChangeToState(newState); }

	// Get this objects name
	virtual std::string GetName() override { return "boss"; }

	// Get this objects position
	virtual Vector2 GetPosition() override { return Vector2{ m_x, m_y }; }

	// Get health
	float GetHealth() { return m_health; }

	// Get max possible health
	const float GetMaxHealth() { return m_kMaxHealth; }

	// Updates this object
	virtual void Update(double deltaTime) override;
	
	// Renders this object
	virtual void Render(SDL_Renderer* pRenderer) override;

	// Trys to move object
	bool TryMove(Vector2 deltaPosition, double speed, double deltaTime);

	// Attempts to move this object
	virtual bool Move(Vector2 direction, double deltaTime, float speedMultiplier = 1) override;

	// When an item first collides with another
	void OnCollisionEntry(ColliderComponent* pOtherCollider);
};

