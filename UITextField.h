#pragma once
#include "GameObject.h"
#include "Vector2.h"
#include <SDL_ttf.h>

class UITextField :
	public GameObject
{
private:
	// Position and size for rendering
	SDL_FRect m_transform;

	// Text texture for rendering
	SDL_Texture* m_pTexture;

public:
	// Constructor/Destructor
	UITextField(TTF_Font* pFont, const char* pText, SDL_Color color,
		Vector2 position, SDL_Renderer* pRenderer);
	~UITextField();
	
	// Sets text position
	void SetTransform(float x, float y) { m_transform.x = x; m_transform.y = y; }

	// Renders this scene
	virtual void Render(SDL_Renderer* pRenderer) override;
};


