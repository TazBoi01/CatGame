#include "Background.h"
#include <iostream>

Background::Background(SDL_Renderer* pRenderer, SDL_Surface* pImageCarpet, int windowW, int windowH, float w, float h)
	:m_windowW(windowW)
	,m_windowH(windowH)
{
	// Set tile width and height
	m_transform.w = w;
	m_transform.h = h;
	
	// If surface is a nullptr, image load fail
	if (pImageCarpet == nullptr)
	{
		std::cout << "Surface load fail: " << SDL_GetError() << std::endl;
	}

	// Loading texture from surface, error check the texture load, and free the surface
	m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pImageCarpet);
	if (m_pTexture == nullptr)
	{
		std::cout << "Texture load fail: " << SDL_GetError() << std::endl;
	}
}

Background::~Background()
{
}

void Background::Update(double)
{
}

////////////////////
// Render Background
void Background::Render(SDL_Renderer* pRenderer)
{
	// Loop to render each tile
	// I is less than the window size divided by tile x position (+1 to account for 0)
	for (int i = 0; i < ((m_windowW * m_windowH) / (m_transform.x + 1)); ++i)
	{
		// Render tile
		SDL_RenderCopyF(pRenderer, m_pTexture, nullptr, &m_transform);

		// Move tile over one position to the right
		m_transform.x += 100;
		// If tile position exceeds window width, go down a row and reset x position
		if (m_transform.x > m_windowW)
		{
			m_transform.y += m_transform.h;
			m_transform.x = 0;
		}
	}

	// Set tile position to 0 for next render
	m_transform.x = 0;
	m_transform.y = 0;
}
