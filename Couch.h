#pragma once
#include "ImageHandler.h"
#include "ColliderComponent.h"
#include <iostream>
#include <string>
#include <SDL.h>

class Couch :
	public GameObject
{
private:
	// X position
	float m_x;
	// Y position
	float m_y;

	// Position and dimensions.
	SDL_FRect m_renderTransform;

	// Collider component.
	ColliderComponent m_collider;

	// Texture for object appearance
	SDL_Texture* m_pTexture;

public:
	Couch(SDL_Renderer* pRenderer, SDL_Surface* pImageCouch, CollisionReferee* pReferee, float w, float h, Vector2 position);
	~Couch();

	// Gets this object's name.
	virtual std::string GetName() override { return "Couch"; }

	virtual void Update(double deltaTime) override;
	virtual void Render(SDL_Renderer* pRenderer) override;
};

