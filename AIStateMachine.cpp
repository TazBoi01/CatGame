#include "AIStateChargeAndReturn.h"
#include "AIStateChase.h"
#include "AIStatePace.h"
#include "AIStateReturn.h"
#include "AIStateMachine.h"
#include "State.h"

AIStateMachine::AIStateMachine(GameObject* pOwner, GameObject* pTarget, StateName initialState)
	: m_pOwner(pOwner)
	, m_pTarget(pTarget)
	, m_pCurrentState(nullptr)
	, m_currentStateName(StateName::None)
{
	ChangeToState(initialState);
}

AIStateMachine::~AIStateMachine()
{
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Exit();
		delete m_pCurrentState;
	}
}

////////////////////////////
// Updates the current state
void AIStateMachine::Update(double deltaTime)
{
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Update(deltaTime);
	}
}

////////////////////////////////////////////////////////////////
// Changes states, won't change if passed state == current state
void AIStateMachine::ChangeToState(StateName state)
{
	constexpr float kMouseSpawnXAndY = 400;
	constexpr float kMouseBoundsXRight = 700;
	constexpr float kMouseBoundsXLeft = 200;
	if (m_currentStateName == state)
		return;
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Exit();
		delete m_pCurrentState;
	}

	switch (state)
	{
	case StateName::Pace:
	{
		m_pCurrentState = new AIStatePace(this, Vector2{ kMouseBoundsXLeft, kMouseBoundsXRight }, 0);
		break;
	}
	case StateName::Chase:
	{
		m_pCurrentState = new AIStateChase(this, m_pTarget);
		break;
	}
	case StateName::Return:
	{
		m_pCurrentState = new AIStateReturn(this, Vector2{ kMouseSpawnXAndY, kMouseSpawnXAndY });
		break;
	}
	case StateName::Charge:
	{
		m_pCurrentState = new AIStateChargeAndReturn(this, m_pTarget);
		break;
	}
	}
	m_currentStateName = state;
	m_pCurrentState->Enter();
}
