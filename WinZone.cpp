#include "WinZone.h"
#include <iostream>
#include <SDL.h>

////////////////////////////////////////////
// Constructor
WinZone::WinZone(SDL_FRect transform, SDL_Renderer* pRenderer, SDL_Surface* pImageFlag,
	CollisionReferee* pReferee)
	: m_renderTransform(transform)
	, m_collider(this, transform, pReferee, true)
{
	// Surface load check
	if (pImageFlag == nullptr)
	{
		std::cout << "Surface load fail: " << SDL_GetError() << std::endl;
	}

	// Loading texture
	m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pImageFlag);
	if (m_pTexture == nullptr)
	{
		std::cout << "Texture load fail: " << SDL_GetError() << std::endl;
	}
}

////////////////////////////////////////////
// Destructor
WinZone::~WinZone()
{
}

////////////////////////////////////////////
// Draws this object as a square
void WinZone::Render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyF(pRenderer, m_pTexture, nullptr, &m_renderTransform);
}

void WinZone::Update(double)
{
}
