#include "UITextField.h"


UITextField::UITextField(TTF_Font* pFont, const char* pText, SDL_Color color, Vector2 position, SDL_Renderer* pRenderer)
	: m_pTexture(nullptr)
{
	// Create surface
	SDL_Surface* pTextSurface = TTF_RenderText_Solid(pFont, pText, color);

	// Create texture
	m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pTextSurface);

	// Get dimensions for transform
	m_transform.x = position.m_x;
	m_transform.y = position.m_y;

	m_transform.w = (float)pTextSurface->w;
	m_transform.h = (float)pTextSurface->h;

	SDL_FreeSurface(pTextSurface);
}

UITextField::~UITextField()
{
	SDL_DestroyTexture(m_pTexture);
}

/////////////////////
// Renders this scene
void UITextField::Render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyF(pRenderer, m_pTexture, nullptr, &m_transform);
}