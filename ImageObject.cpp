
#include "ImageObject.h"
#include "SDL.h"
#include <iostream>

ImageObject::ImageObject(SDL_Renderer* pRenderer, SDL_Surface* pImageSurface, int numberObjects, float w, float h)
	:m_pTexture(nullptr)
	,m_numberObjects(numberObjects)
	,m_transformVector(numberObjects)
{

	// For how many opjects to render, complete loop
	for (int i = 0; i < numberObjects; ++i)
	{
		m_transformVector.push_back(SDL_FRect());

		// Set objects in random place
		m_x = 0;
		m_y = 0;

		// Take x, y, w, and h to create/place an image object
		m_transformVector[i].x = m_x;
		m_transformVector[i].y = m_y;
		m_transformVector[i].w = w;
		m_transformVector[i].h = h;

		// Surface load check
		if ( pImageSurface == nullptr)
		{
			std::cout << "Surface load fail: " << SDL_GetError() << std::endl;
		}

		// Loading texture
		m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pImageSurface);
		if (m_pTexture == nullptr)
		{
			std::cout << "Texture load fail: " << SDL_GetError() << std::endl;
		}
	}
}

/////////////
// Destructor
ImageObject::~ImageObject()
{
	SDL_DestroyTexture(m_pTexture);
}

void ImageObject::Update(double)
{
}

/////////////////////
// Render all objects
void ImageObject::Render(SDL_Renderer* pRenderer)
{
	// Iterate through all objects
	for (int i = 0; i < m_numberObjects; ++i)
	{
		SDL_RenderCopyF(pRenderer, m_pTexture, nullptr, &m_transformVector[i]);
	}
}