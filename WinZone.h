#pragma once
#include "GameObject.h"
#include "ColliderComponent.h"
#include <SDL.h>

class WinZone :
	public GameObject
{
private:
	// Position and dimensions.
	SDL_FRect m_renderTransform;

	// Collider component.
	ColliderComponent m_collider;

	// Texture for object appearance
	SDL_Texture* m_pTexture;
public:
	WinZone(SDL_FRect transform, SDL_Renderer* pRenderer, SDL_Surface* pImageFlag, CollisionReferee* pReferee);
	~WinZone();

	// Gets this object's name.
	virtual std::string GetName() override { return "winZone"; }

	virtual void Render(SDL_Renderer* pRenderer) override;
	virtual void Update(double deltaTime) override;
};

