#include "HealthBar.h"
#include "SDL.h"

// Constructor/Destructor
HealthBar::HealthBar(Vector2 position, SDL_Renderer* pRenderer, SDL_Surface* pBarSurface, const float maxHealth)
	: m_kMaxHealth(maxHealth)
	, m_animation(12, 100, 10, 10, pRenderer, pBarSurface)
{
	// Sets animation sequences with name, first frame, and last frame. Starting full health, going down
	m_animation.AddAnimationSequence("10", 0, 0);
	m_animation.AddAnimationSequence("9", 1, 1);
	m_animation.AddAnimationSequence("8", 2, 2);
	m_animation.AddAnimationSequence("7", 3, 3);
	m_animation.AddAnimationSequence("6", 4, 4);
	m_animation.AddAnimationSequence("5", 5, 5);
	m_animation.AddAnimationSequence("4", 6, 6);
	m_animation.AddAnimationSequence("3", 7, 7);
	m_animation.AddAnimationSequence("2", 8, 8);
	m_animation.AddAnimationSequence("1", 9, 9);

	// Sets initial position
	m_animation.SetTransformX(position.m_x);
	m_animation.SetTransformY(position.m_y);
}

HealthBar::~HealthBar()
{
}

////////////////////////////////////////////////////////
// Checks given health with max health, sets current bar
void HealthBar::CheckHealth(float m_health)
{
	// Percentages
	constexpr float k90Percent = .9f;
	constexpr float k80Percent = .8f;
	constexpr float k70Percent = .7f;
	constexpr float k60Percent = .6f;
	constexpr float k50Percent = .5f;
	constexpr float k40Percent = .4f;
	constexpr float k30Percent = .3f;
	constexpr float k20Percent = .2f;
	constexpr float k10Percent = .1f;

	if (m_health > (m_kMaxHealth * k90Percent))
	{
	m_animation.PlayAnimation("10");
	}
	else if (m_health > (m_kMaxHealth * k80Percent))
	{
		m_animation.PlayAnimation("9");
	}
	else if (m_health > (m_kMaxHealth * k70Percent))
	{
		m_animation.PlayAnimation("8");
	}
	else if (m_health > (m_kMaxHealth * k60Percent))
	{
		m_animation.PlayAnimation("7");
	}
	else if (m_health > (m_kMaxHealth * k50Percent))
	{
		m_animation.PlayAnimation("6");
	}
	else if (m_health > (m_kMaxHealth * k40Percent))
	{
		m_animation.PlayAnimation("5");
	}
	else if (m_health > (m_kMaxHealth * k30Percent))
	{
		m_animation.PlayAnimation("4");
	}
	else if (m_health > (m_kMaxHealth * k20Percent))
	{
		m_animation.PlayAnimation("3");
	}
	else if (m_health > (m_kMaxHealth * k10Percent))
	{
		m_animation.PlayAnimation("2");
	}
	else
	{
		m_animation.PlayAnimation("1");
	}
}

/////////////////////
// Renders this scene
void HealthBar::Render(SDL_Renderer* pRenderer)
{
	m_animation.Render(pRenderer);
}