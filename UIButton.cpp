#include "UIButton.h"

UIButton::UIButton(SDL_FRect transform, UITextField* text, ImageHandler* pImageHandler,
	SDL_Renderer* pRenderer, SoundReferee* pSoundReferee)
	: m_transform(transform)
	, m_pButtonTexture(nullptr)
	, m_pButtonHighlightedTexture(nullptr)
	, m_text(text)
	, m_isHighlighted(false)
	, m_pSoundReferee(pSoundReferee)
	, m_pImageHandler(pImageHandler)
	, m_activeChannel(-1)
{
	m_pButtonTexture = SDL_CreateTextureFromSurface(pRenderer, pImageHandler->GetImageSurface("button"));
	m_pButtonHighlightedTexture = SDL_CreateTextureFromSurface(pRenderer, pImageHandler->GetImageSurface("buttonHighlighted"));
}

UIButton::~UIButton()
{
	SDL_DestroyTexture(m_pButtonTexture);
	SDL_DestroyTexture(m_pButtonHighlightedTexture);
	delete m_text;
}

///////////////////////////////////////////////////////////
// Triggers the button's callback as if it had been clicked
void UIButton::Trigger()
{
	// Play sound when button clicked
	if (m_activeChannel == -1)
	{
		m_activeChannel = m_pSoundReferee->PlaySound("click", m_activeChannel, 0);
	}
	else
	{
		Mix_HaltChannel(m_activeChannel);
		m_activeChannel = -1;
	}
	// If there is a callback, do callback
	if (m_callback != nullptr)
		m_callback();
}

//////////////////////
// Renders this object
void UIButton::Render(SDL_Renderer* pRenderer)
{
	if (m_isHighlighted)
		SDL_RenderCopyF(pRenderer, m_pButtonHighlightedTexture, nullptr, &m_transform);
	else
		SDL_RenderCopyF(pRenderer, m_pButtonTexture, nullptr, &m_transform);
	m_text->Render(pRenderer);
}


//////////////////////////////////////////////
// Handles current event, returns true if quit
void UIButton::HandleEvent(SDL_Event* pEvent)
{
	switch (pEvent->type)
	{
	// If hover
	case SDL_MOUSEMOTION:
		m_isHighlighted = HitTest(pEvent->button.x, pEvent->button.y);
		break;
	// If click
	case SDL_MOUSEBUTTONDOWN:
		if (m_callback != nullptr && HitTest(pEvent->button.x, pEvent->button.y))
			Trigger();
		break;
	}
}

////////////////////////////////////////////////////////////////////
// Returns whether the given x,y are within the bounds of the button
bool UIButton::HitTest(int x, int y)
{
	return (x > m_transform.x && x < m_transform.x + m_transform.w && 
			y > m_transform.y && y < m_transform.y + m_transform.h);
}
