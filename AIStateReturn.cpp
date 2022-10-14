#include "AIStateMachine.h"
#include "AIStateReturn.h"
#include "GameObject.h"

AIStateReturn::AIStateReturn(AIStateMachine* pOwner, Vector2 homePosition)
	: m_pOwner(pOwner)
	, m_homePosition(homePosition)
{
}

AIStateReturn::~AIStateReturn()
{
}

/////////////////////
// Updates this state
void AIStateReturn::Update(double deltaTime)
{
	// Finds objects path towards "Home". As soon as they get there, set state to Pace
	Vector2 pathToTarget = m_homePosition - m_pOwner->GetOwner()->GetPosition();
	pathToTarget = pathToTarget.GetNormalized();
	m_pOwner->GetOwner()->Move(pathToTarget, deltaTime);
	// Check if x is within the bounds of returning (+2 and -2 of home x)
	if ((int)m_pOwner->GetOwner()->GetPosition().m_x < ((int)m_homePosition.m_x + 2) &&
		(int)m_pOwner->GetOwner()->GetPosition().m_x > ((int)m_homePosition.m_x - 2))
	{
		// Check if y is within the bounds of returning (+2 and -2 of home y)
		if ((int)m_pOwner->GetOwner()->GetPosition().m_y < ((int)m_homePosition.m_y + 2) &&
			(int)m_pOwner->GetOwner()->GetPosition().m_y > ((int)m_homePosition.m_y - 2))
		{
				m_pOwner->ChangeToState(AIStateMachine::StateName::Pace);
		}
	}
}
