#include "CatGameStateMachine.h"
#include "SDL.h"
#include "StateWin.h"
#include <iostream>
#include <cmath>

StateWin::StateWin(CatGameStateMachine* pOwner, TTF_Font* pFont)
	: m_pOwner(pOwner)
	, m_keyboardButtonIndex(-1)
	, m_pImageHandler(pOwner->GetGame()->GetImageHandler())
{
	constexpr int kFirstButtonX = 75;
	constexpr int kFirstButtonY = 300;
	constexpr int kSecondButtonX = 300;
	constexpr int kSecondButtonY = 300;
	constexpr int kThirdButtonX = 525;
	constexpr int kThirdButtonY = 300;
	constexpr int kButtonW = 200;
	constexpr int kButtonH = 50;

	// Title image
	m_gameObjects.push_back(new ImageObject(pOwner->GetGame()->GetRenderer(), m_pImageHandler->GetImageSurface("win"), 1, 800, 600));
	// Add main menu button, set callback, and add to gameObject vector
	UIButton* pMainMenuButton = new UIButton(SDL_FRect{ kFirstButtonX, kFirstButtonY, kButtonW, kButtonH },
		new UITextField{ pFont, "Main Menu", SDL_Color{ 0, 0, 0 }, Vector2{ kFirstButtonX, kFirstButtonY }, pOwner->GetGame()->GetRenderer() },
		pOwner->GetGame()->GetImageHandler(), pOwner->GetGame()->GetRenderer(), pOwner->GetGame()->GetSoundReferee());

	pMainMenuButton->SetCallback([pOwner]()->void
		{
			pOwner->LoadScene(CatGameStateMachine::SceneName::MainMenu);
		});
	m_gameObjects.push_back(pMainMenuButton);
	// Add reply button, set callback, and add to gameObject vector
	UIButton* pReplayButton = new UIButton(SDL_FRect{ kSecondButtonX, kSecondButtonY, kButtonW, kButtonH },
		new UITextField{ pFont, "Replay", SDL_Color{ 0, 0, 0 }, Vector2{ kSecondButtonX, kSecondButtonY }, pOwner->GetGame()->GetRenderer() },
		pOwner->GetGame()->GetImageHandler(), pOwner->GetGame()->GetRenderer(), pOwner->GetGame()->GetSoundReferee());

	pReplayButton->SetCallback([pOwner]()->void
		{
			pOwner->LoadScene(CatGameStateMachine::SceneName::Gameplay);
		});
	m_gameObjects.push_back(pReplayButton);

	// Add quit button, set callback, and add to gameObject vector
	UIButton* pQuitButton = new UIButton(SDL_FRect{ kThirdButtonX, kThirdButtonY, kButtonW, kButtonH },
		new UITextField{ pFont, "Quit", SDL_Color{ 0, 0, 0 }, Vector2{ kThirdButtonX, kThirdButtonY }, pOwner->GetGame()->GetRenderer() },
		pOwner->GetGame()->GetImageHandler(), pOwner->GetGame()->GetRenderer(), pOwner->GetGame()->GetSoundReferee());

	pQuitButton->SetCallback([pOwner]()->void
		{
			pOwner->GetGame()->Quit();
		});
	m_gameObjects.push_back(pQuitButton);

	// Store buttons in button set
	m_buttonSet.push_back(pMainMenuButton);
	m_buttonSet.push_back(pQuitButton);
	m_buttonSet.push_back(pReplayButton);

}

StateWin::~StateWin()
{
	for (GameObject* object : m_gameObjects)
	{
		delete object;
	}
}

//////////////////////
// Renders this scene
void StateWin::Render(SDL_Renderer* pRenderer)
{
	SDL_RenderClear(pRenderer);
	for (GameObject* object : m_gameObjects)
	{
		object->Render(pRenderer);
	}
	SDL_RenderPresent(pRenderer);
}

//////////////////////////////////////////////
// Handles current event, returns true if quit
bool StateWin::HandleEvent(SDL_Event* pEvent)
{
	switch (pEvent->type)
	{
	case SDL_WINDOWEVENT:
		if (pEvent->window.event == SDL_WINDOWEVENT_CLOSE)
			return true;
		break;

		// Keyboard events for button scrolling
	case SDL_KEYDOWN:
		switch (pEvent->key.keysym.scancode)
		{

		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			ChangeButtonFocus(-1);
			break;
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			ChangeButtonFocus(1);
			break;
		case SDL_SCANCODE_RETURN:
			if (m_keyboardButtonIndex != -1)
				m_buttonSet[m_keyboardButtonIndex]->Trigger();
			break;
		}
		break;

		// Mouse moving resets keyboard index
	case SDL_MOUSEMOTION:
		m_keyboardButtonIndex = -1;
		break;
	}


	for (GameObject* object : m_gameObjects)
	{
		object->HandleEvent(pEvent);
	}

	return false;
}

///////////////////////////////////////////
// Changes button focus when using keyboard
void StateWin::ChangeButtonFocus(int direction)
{

	m_keyboardButtonIndex = (m_keyboardButtonIndex + direction) % (int)m_buttonSet.capacity();
	
	if (m_keyboardButtonIndex < 0)
	{
		m_keyboardButtonIndex = (int)m_buttonSet.capacity();
	}

	for (int i = 0; i < (int)m_buttonSet.capacity(); i++)
	{
			m_buttonSet[i]->SetIsHighlighted(i == m_keyboardButtonIndex);
	}
}