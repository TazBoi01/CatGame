#include "CatGameStateMachine.h"
#include "SDLGame.h"
#include "StateBoss.h"
#include "StateGameplay.h"
#include "StateLoad.h"
#include "StateMainMenu.h"
#include "StateNewGame.h"
#include "StateWin.h"
#include "StateLose.h"

//////////////
// Constructor
CatGameStateMachine::CatGameStateMachine(SDLGame* pOwner)
	: m_pOwner(pOwner)
	, m_pCurrentState(nullptr)
	, m_pNextState(nullptr)
	, m_pFont(nullptr)
	, m_index(0)
{
}

/////////////
// Destructor
CatGameStateMachine::~CatGameStateMachine()
{
}

////////////////////////////////////////////////////
// Initializes state machine, returns false if error
bool CatGameStateMachine::Init()
{
	m_pFont = TTF_OpenFont("assets/arial.ttf", 40);

	LoadScene(SceneName::MainMenu);
	return true;
}

////////////////////////////
// Updates the current scene
void CatGameStateMachine::UpdateCurrentState(double deltaTime)
{
	// Perform actual state change
	if (m_pNextState != nullptr)
	{
		ChangeState(m_pNextState);
		m_pNextState = nullptr;
	}

	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Update(deltaTime);
	}
}

////////////////////////////
// Renders the current scene
void CatGameStateMachine::RenderCurrentState(SDL_Renderer* pRenderer)
{
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Render(pRenderer);
	}
}

//////////////////////////////////////////////
// Handles current event, returns true if quit
bool CatGameStateMachine::HandleEvent(SDL_Event* pEvent)
{
	if (m_pCurrentState == nullptr)
		return false;
	else
		return m_pCurrentState->HandleEvent(pEvent);
}

////////////////////////////////////////
// Exits current state, enters new state
void CatGameStateMachine::ChangeState(GameState* pNewState)
{
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Exit();
	}

	delete m_pCurrentState;
	m_pCurrentState = pNewState;
	pNewState->Enter();
}

/////////////////////////////////////
// Load the given scene on next frame
void CatGameStateMachine::LoadScene(SceneName scene)
{
	switch (scene)
	{
	case SceneName::MainMenu:
		// Defer state change until next frame
		m_pNextState = new StateMainMenu(this, m_pFont);
		break;
	case SceneName::NewGame:
		// Defer state change until next frame
		m_pNextState = new StateNewGame(this, m_pFont);
		break;
	case SceneName::Load:
		// Defer state change until next frame
		m_pNextState = new StateLoad(this, m_pFont);
		break;
	case SceneName::Gameplay:
		// Defer state change until next frame
		m_pNextState = new StateGameplay(this);
		break;
	case SceneName::Boss:
		// Defer state change until next frame
		m_pNextState = new StateBoss(this);
		break;
	case SceneName::Win:
		// Defer state change until next frame
		m_pNextState = new StateWin(this, m_pFont);
		break; 
	case SceneName::Lose:
		// Defer state change until next frame
		m_pNextState = new StateLose(this, m_pFont);
		break;
	}
}

bool CatGameStateMachine::Cleanup()
{
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Exit();
		delete m_pCurrentState;
	}
	return true;
}
