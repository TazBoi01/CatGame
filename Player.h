#pragma once
#include "GameObject.h"
#include "AnimationComponent.h"
#include "ColliderComponent.h"
#include "HealthBar.h"
#include "ParticleSystem.h"
#include "SoundReferee.h"

class Player :
	public GameObject
{
private:
	// Max possible health
	const float m_kMaxHealth = 500;

	// Normal speed
	const float m_kNormalSpeed = 100;

	// Health UI
	HealthBar m_healthBar;

	// Particle effect trail
	ParticleSystem* m_pParticleTrail;

	// Particle Texture
	SDL_Texture* m_pParticleTexture;
	
	// Player health
	float m_health;

	// Movement speed.
	double m_Speed;

	// Position horizontal
	float m_x;

	// Position vertical
	float m_y;

	// Current direction to move (-1 left, 1 right)
	int m_horizontalDirection;

	//(-1 up, 1 down)
	int m_verticalDirection;

	// Allocates an SDL_FRect for rendering.
	SDL_FRect m_transform;

	// Allocate animation component
	AnimationComponent m_animation;

	// Allocate collider component
	ColliderComponent m_collider;

	// Store Sound referee
	SoundReferee* m_pSoundReferee;

	// Array to store sound channels
	int m_activeChannels[4];

	// If win condition met, true. Detected in StateGameplay::Update()
	bool m_winCondition;

	// Check speed to determine if sprinting
	bool m_isSprinting;

	// How many catnips collected in boss state
	size_t m_catnips;

public:
	// Constructor that takes initial position to be placed
	Player(SDL_Renderer* pRenderer, SDL_Surface* pImagePlayer, SDL_Surface* pImageBar, SDL_Surface* pImageParticle, float x, float y,
		CollisionReferee* pReferee, SoundReferee* pSoundReferee);
	// Destructor
	~Player();

	// Sets which direction player is moving
	void SetVerticalDirection(int dir) { m_verticalDirection = dir; }
	void SetHorizontalDirection(int dir) { m_horizontalDirection = dir; }

	// Set movement speed
	void SetSpeed(double speed) { m_Speed = speed; }

	// Set player health
	void SetHealth(float health) { m_health = health; }

	// Get health
	float GetHealth() { return m_health; }

	// Get if win condition met
	bool GetWinCondition() { return m_winCondition; }

	// Get number of catnips owned
	size_t GetCatnips() { return m_catnips; }

	// Inherited via GameObject
	virtual void Update(double deltaTime) override;
	virtual void Render(SDL_Renderer* pRenderer) override;
	virtual std::string GetName() override { return "player"; }
	virtual Vector2 GetPosition() override { return Vector2{ m_x, m_y }; }

	// Process player input
	bool ProcessInput();

	// Attempt to move object, return whether successful or not
	bool TryMove(Vector2 deltaPosition, double speed, double deltaTime);

	// When an item first collides with another
	void OnCollisionEntry(ColliderComponent* pOtherCollider);

	// When collides with item, continuous
	void OnCollisionUpdate(ColliderComponent* pOtherCollider);

private:
	// Process continuous keyboard key presses
	bool ProcessContinuousKeys(const Uint8* pKeyState);
};