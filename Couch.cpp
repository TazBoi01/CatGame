#include "Couch.h"

////////////////////////////////////////////
// Constructor
Couch::Couch(SDL_Renderer* pRenderer, SDL_Surface* pImageCouch, CollisionReferee* pReferee, float w, float h, Vector2 position)
	: m_collider(this, m_renderTransform, pReferee, false)
{

	// Randomly place items
	m_x = position.m_x;
	m_y = position.m_y;

	// Set position for collider
	m_renderTransform.x = round(m_x);
	m_renderTransform.y = round(m_y);
	m_renderTransform.w = w;
	m_renderTransform.h = h;
	m_collider.SetSize(Vector2{ (float)w, (float)h });
	m_collider.SetPosition(Vector2{ m_x, m_y });

	// Surface load check
	if (pImageCouch == nullptr)
	{
		std::cout << "Surface load fail: " << SDL_GetError() << std::endl;
	}

	// Loading texture
	m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pImageCouch);
	if (m_pTexture == nullptr)
	{
		std::cout << "Texture load fail: " << SDL_GetError() << std::endl;
	}
}

////////////////////////////////////////////
// Destructor
Couch::~Couch()
{
}

void Couch::Update(double)
{
}

////////////////////////////////////////////
// Renders object
void Couch::Render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyF(pRenderer, m_pTexture, nullptr, &m_renderTransform);
}
