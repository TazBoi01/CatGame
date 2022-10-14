#include "CatGameStateMachine.h"
#include "SDL.h"
#include "SDLGame.h"
#include "StateNewGame.h"
#include "Vector2.h"

StateNewGame::StateNewGame(CatGameStateMachine* pOwner, TTF_Font* pFont)
	: m_pOwner(pOwner)
	, m_gameObjects()
	, m_buttonSet()
	, m_keyboardButtonIndex(-1)
	, m_pImageHandler(pOwner->GetGame()->GetImageHandler())
{
	constexpr int kBackButtonX = 75;
	constexpr int kBackButtonY = 500;
	constexpr int kFirstButtonX = 300;
	constexpr int kFirstButtonY = 300;
	constexpr int kSecondButtonX = 300;
	constexpr int kSecondButtonY = 400;
	constexpr int kThirdButtonX = 300;
	constexpr int kThirdButtonY = 500;
	constexpr int kButtonW = 200;
	constexpr int kButtonH = 50;
	// Load screen image
	m_gameObjects.push_back(new ImageObject(pOwner->GetGame()->GetRenderer(), m_pImageHandler->GetImageSurface("menu"), 1, 800, 600));
	
	// Back Button
	UIButton* pBackButton = new UIButton(SDL_FRect{ kBackButtonX, kBackButtonY, kButtonW, kButtonH },
		new UITextField{ pFont, "Back", SDL_Color{ 0, 0, 0 }, Vector2{ kBackButtonX, kBackButtonY }, pOwner->GetGame()->GetRenderer() },
		m_pImageHandler, pOwner->GetGame()->GetRenderer(), pOwner->GetGame()->GetSoundReferee());

	pBackButton->SetCallback([pOwner, pBackButton]()->void
		{
			pOwner->LoadScene(CatGameStateMachine::SceneName::MainMenu);
		});
	m_gameObjects.push_back(pBackButton);

	// Load Button
	UIButton* pLoadButton = new UIButton(SDL_FRect{ kFirstButtonX, kFirstButtonY, kButtonW, kButtonH },
		new UITextField{ pFont, "New 1", SDL_Color{ 0, 0, 0 }, Vector2{ kFirstButtonX, kFirstButtonY }, pOwner->GetGame()->GetRenderer() },
		pOwner->GetGame()->GetImageHandler(), pOwner->GetGame()->GetRenderer(), pOwner->GetGame()->GetSoundReferee());

	pLoadButton->SetCallback([pOwner]()->void
		{
			constexpr size_t index = 0;
			pOwner->SetIndex(index);
			pOwner->LoadScene(CatGameStateMachine::SceneName::Gameplay);
		});
	m_gameObjects.push_back(pLoadButton);

	// Load Button
	UIButton* pLoadButton1 = new UIButton(SDL_FRect{ kSecondButtonX, kSecondButtonY, kButtonW, kButtonH },
		new UITextField{ pFont, "New 2", SDL_Color{ 0, 0, 0 }, Vector2{ kSecondButtonX, kSecondButtonY }, pOwner->GetGame()->GetRenderer() },
		pOwner->GetGame()->GetImageHandler(), pOwner->GetGame()->GetRenderer(), pOwner->GetGame()->GetSoundReferee());

	pLoadButton1->SetCallback([pOwner]()->void
		{
			constexpr size_t index = 1;
			pOwner->SetIndex(index);
			pOwner->LoadScene(CatGameStateMachine::SceneName::Gameplay);
		});
	m_gameObjects.push_back(pLoadButton1);

	// Load save 3
	UIButton* pLoadButton2 = new UIButton(SDL_FRect{ kThirdButtonX, kThirdButtonY, kButtonW, kButtonH },
		new UITextField{ pFont, "New 3", SDL_Color{ 0, 0, 0 }, Vector2{ kThirdButtonX, kThirdButtonY }, pOwner->GetGame()->GetRenderer() },
		pOwner->GetGame()->GetImageHandler(), pOwner->GetGame()->GetRenderer(), pOwner->GetGame()->GetSoundReferee());

	pLoadButton2->SetCallback([pOwner]()->void
		{
			constexpr size_t index = 2;
			pOwner->SetIndex(index);
			pOwner->LoadScene(CatGameStateMachine::SceneName::Gameplay);
		});
	m_gameObjects.push_back(pLoadButton2);

	// Store buttons in button set
	m_buttonSet.push_back(pLoadButton);
	m_buttonSet.push_back(pLoadButton1);
	m_buttonSet.push_back(pLoadButton2);
	m_buttonSet.push_back(pBackButton);
}


/////////////
// Destructor
StateNewGame::~StateNewGame()
{
	for (GameObject* object : m_gameObjects)
	{
		delete object;
	}
}

/////////////////////
// Renders this scene
void StateNewGame::Render(SDL_Renderer* pRenderer)
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
bool StateNewGame::HandleEvent(SDL_Event* pEvent)
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
void StateNewGame::ChangeButtonFocus(int direction)
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
