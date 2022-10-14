#pragma once
#include "GameObject.h"
#include "HealthBar.h"
#include "AIStateMachine.h"
#include "AnimationComponent.h"
#include "ColliderComponent.h"

class Enemy :
	public GameObject
{
private:
	// Enemy detection radius for when target detected
	const float m_kDetectDistanceSqr = 200 * 200;

	// Max possible health
	const float m_kMaxHealth = 200;

	// Health UI
	HealthBar m_healthBar;

	// Enemy health
	float m_health;

	// Movement speed.
	double m_Speed = 100;

	// Position horizontal
	float m_x;

	// Position vertical
	float m_y;

	// Current direction to move (-1 left, 1 right)
	float m_horizontalDirection;

	//(-1 up, 1 down)
	float m_verticalDirection;

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

	// Behavior state machine
	AIStateMachine m_stateMachine;

	// Pointer to enemy's target
	GameObject* m_pTarget;

public:
	// Constructor that takes initial position to be placed
	Enemy(SDL_Renderer* pRenderer, SDL_Surface* pImagePlayer, SDL_Surface* pImageBar, float x, float y, CollisionReferee* pReferee, GameObject* pTarget);
	// Destructor
	~Enemy();

	// Sets which direction player is moving
	void SetVerticalDirection(int dir) { m_verticalDirection = (float)dir; }
	void SetHorizontalDirection(int dir) { m_horizontalDirection = (float)dir; }
	// Set current animation
	void SetCurrentAnimation(std::string currentAnimation) { m_currentAnimation = currentAnimation; }
	// Set enemys health
	void SetHealth(float newHealth) { m_health = newHealth; }
	// Get max possible health
	const float GetMaxHealth() { return m_kMaxHealth; }
	// Get health
	const float GetHealth() { return m_health; }

	// Inherited via GameObject
	virtual void Update(double deltaTime) override;
	virtual void Render(SDL_Renderer* pRenderer) override;
	virtual std::string GetName() override { return "mouse"; }
	virtual Vector2 GetPosition() override { return Vector2{ m_x, m_y }; }

	// Attempt to move object, return whether successful or not
	bool TryMove(Vector2 deltaPosition, double speed, double deltaTime);

	// When collides with item
	void OnCollisionUpdate(ColliderComponent* pOtherCollider);

	// Attempts to move object
	virtual bool Move(Vector2 direction, double deltaTime, float speedMultiplier = 1) override;
};

