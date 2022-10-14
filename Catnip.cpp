#include "Catnip.h"
#include <SDL.h>
#include <iostream>

Catnip::Catnip(SDL_Renderer* pRenderer, SDL_Surface* pImageCatnip, float w, float h,
	int frameRate, int frameCount, CollisionReferee* pReferee, Vector2 position)
	: m_animation(frameRate, w, h, frameCount, pRenderer, pImageCatnip)
	, m_collider(this, m_animation.GetTransform(), pReferee, true)
{

	// Randomly place items
	m_x = position.m_x;
	m_y = position.m_y;

	// Take x, y, w, and h to create/place an image object
	m_animation.SetTransformX(m_x);
	m_animation.SetTransformY(m_y);
	m_collider.SetPosition(Vector2{ m_x, m_y });

	// Add animation sequence
	m_animation.AddAnimationSequence("catnip", 0, 1);

	// Set and play the animation
	m_currentAnimation = "catnip";
	m_animation.PlayAnimation("catnip");
}

Catnip::~Catnip()
{
}

///////////////////
// Update animation
void Catnip::Update(double deltaTime)
{
	m_animation.Update(deltaTime);
}

////////////////
// Render object
void Catnip::Render(SDL_Renderer* pRenderer)
{
	m_animation.Render(pRenderer);
}