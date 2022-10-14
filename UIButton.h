#pragma once
#include "ImageHandler.h"
#include "SoundReferee.h"
#include "UITextField.h"
#include <functional>
#include <string>

/////////////////////////////////////////////////////////////////////
// A clickable button.
/////////////////////////////////////////////////////////////////////
class UIButton :
	public GameObject
{
private:
	// Position and size TODO REPLACE
	SDL_FRect m_transform;

	// Button neutral texture
	SDL_Texture* m_pButtonTexture;

	// Button highlighted texture
	SDL_Texture* m_pButtonHighlightedTexture;

	// Function to be called when this button is clicked.
	std::function<void()> m_callback;

	// Text on button
	UITextField* m_text;

	// Whether this button is currently highlighted.
	bool m_isHighlighted;

	// Store Sound referee
	SoundReferee* m_pSoundReferee;

	// Store Image Handler
	ImageHandler* m_pImageHandler;

	// int to store channel
	int m_activeChannel;

public:
	UIButton(SDL_FRect transform, UITextField* text, ImageHandler* pImageHandler,
		SDL_Renderer* pRenderer, SoundReferee* pSoundReferee);
	~UIButton();

	// Triggers the button's callback as if it had been clicked
	void Trigger();

	// Sets this button's callback function
	void SetCallback(std::function<void()> callback) { m_callback = callback; }

	void SetIsHighlighted(bool ifHighlighted) { m_isHighlighted = ifHighlighted; }

	// Renders this object
	virtual void Render(SDL_Renderer* pRenderer) override;

	// Handles current event, returns true if quit
	virtual void HandleEvent(SDL_Event* pEvent) override;

private:
	// Returns whether the given x,y are within the bounds of the button.
	bool HitTest(int x, int y);
};

