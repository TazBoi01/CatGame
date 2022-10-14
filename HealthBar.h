#pragma once
#include "AnimationComponent.h"
#include "GameObject.h"
#include "Vector2.h"
#include <SDL_ttf.h>

class HealthBar :
	public GameObject
{
private:
	// Object's owners max health
	const float m_kMaxHealth;

	// Animation component
	AnimationComponent m_animation;
public:
	// Constructor/Destructor
	HealthBar(Vector2 position, SDL_Renderer* pRenderer, SDL_Surface* pBarSurface, const float maxHealth);
	~HealthBar();

	// Sets bar position
	void SetTransform(float x, float y) { m_animation.SetTransformX(x); m_animation.SetTransformY(y); }
	
	// Checks given health with max health, sets current bar
	void CheckHealth(float m_health);

	// Renders this object
	virtual void Render(SDL_Renderer* pRenderer) override;
};
