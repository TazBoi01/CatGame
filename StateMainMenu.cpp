#include "CatGameStateMachine.h"
#include "SDL.h"
#include "SDLGame.h"
#include "StateMainMenu.h"
#include "Vector2.h"

StateMainMenu::StateMainMenu(CatGameStateMachine* pOwner, TTF_Font* pFont)
	: m_pOwner(pOwner)
	, m_gameObjects()
	, m_buttonSet()
	, m_keyboardButtonIndex(-1)
	, m_pImageHandler(pOwner->GetGame()->GetImageHandler())
{
	constexpr int kFirstButtonX = 300;
	constexpr int kFirstButtonY = 300;
	constexpr int kSecondButtonX = 300;
	constexpr int kSecondButtonY = 400;
	constexpr int kThirdButtonX = 300;
	constexpr int kThirdButtonY = 500;
	constexpr int kButtonW = 200;
	constexpr int kButtonH = 50;
	// Title image
	m_gameObjects.push_back(new ImageObject(pOwner->GetGame()->GetRenderer(), m_pImageHandler->GetImageSurface("menu"), 1, 800, 600));
	// Play Button
	UIButton* pPlayButton = new UIButton(SDL_FRect{ kFirstButtonX, kFirstButtonY, kButtonW, kButtonH },
		new UITextField{ pFont, "New", SDL_Color{ 0, 0, 0 }, Vector2{ kFirstButtonX, kFirstButtonY }, pOwner->GetGame()->GetRenderer() },
		m_pImageHandler, pOwner->GetGame()->GetRenderer(), pOwner->GetGame()->GetSoundReferee());

	pPlayButton->SetCallback([pOwner]()->void
		{
			pOwner->LoadScene(CatGameStateMachine::SceneName::NewGame);
		});
	m_gameObjects.push_back(pPlayButton);

	// Load button
	UIButton* pLoadButton = new UIButton(SDL_FRect{ kSecondButtonX, kSecondButtonY, kButtonW, kButtonH },
		new UITextField{ pFont, "Load", SDL_Color{ 0, 0, 0 }, Vector2{ kSecondButtonX, kSecondButtonY }, pOwner->GetGame()->GetRenderer() },
		m_pImageHandler, pOwner->GetGame()->GetRenderer(), pOwner->GetGame()->GetSoundReferee());

	pLoadButton->SetCallback([pOwner]()->void
		{
			pOwner->LoadScene(CatGameStateMachine::SceneName::Load);
		});
	m_gameObjects.push_back(pLoadButton);


	// Quit Button
	UIButton* pQuitButton = new UIButton(SDL_FRect{ kThirdButtonX, kThirdButtonY, kButtonW, kButtonH },
		new UITextField{pFont, "Quit", SDL_Color{ 0, 0, 0 }, Vector2{ kThirdButtonX, kThirdButtonY }, pOwner->GetGame()->GetRenderer() },
		m_pImageHandler, pOwner->GetGame()->GetRenderer(), pOwner->GetGame()->GetSoundReferee());

	pQuitButton->SetCallback([pOwner]()->void
		{
			pOwner->GetGame()->Quit();
		});
	m_gameObjects.push_back(pQuitButton);

	// Store buttons in button set
	m_buttonSet.push_back(pPlayButton);
	m_buttonSet.push_back(pLoadButton);
	m_buttonSet.push_back(pQuitButton);
}


/////////////
// Destructor
StateMainMenu::~StateMainMenu()
{
	for (GameObject* object : m_gameObjects)
	{ 
		delete object;
	}
}

/////////////////////
// Renders this scene
void StateMainMenu::Render(SDL_Renderer* pRenderer)
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
bool StateMainMenu::HandleEvent(SDL_Event* pEvent)
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

		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			ChangeButtonFocus(1);
			break;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			ChangeButtonFocus(-1);
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
void StateMainMenu::ChangeButtonFocus(int direction)
{
	m_keyboardButtonIndex = (m_keyboardButtonIndex + direction) % (int)m_buttonSet.capacity();
	if (m_keyboardButtonIndex < 0)
	{
		m_keyboardButtonIndex = (int)m_buttonSet.capacity();
	}
	for (size_t i = 0; i < m_buttonSet.capacity(); i++)
	{
		m_buttonSet[i]->SetIsHighlighted(i == m_keyboardButtonIndex);
	}
}
