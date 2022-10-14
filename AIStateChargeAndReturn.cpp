#include "AIStateChargeAndReturn.h"

AIStateChargeAndReturn::AIStateChargeAndReturn(AIStateMachine* pOwner, GameObject* pTarget)
	: m_pOwner(pOwner)
	, m_timer(2)
	, m_pTarget(pTarget)
	, m_targetPosition(pTarget->GetPosition())
{
}

AIStateChargeAndReturn::~AIStateChargeAndReturn()
{
}

void AIStateChargeAndReturn::Update(double deltaTime)
{
	m_timer -= deltaTime;
	if (m_timer <= 0)
	{
		// Charge quickly at a snapshot of the players pos, then return back
		Vector2 pathToTarget = m_targetPosition - m_pOwner->GetOwner()->GetPosition();
		pathToTarget = pathToTarget.GetNormalized();
		m_pOwner->GetOwner()->Move(pathToTarget, deltaTime, 4);

		// If reach target (truncated), return
		if ((int)m_pOwner->GetOwner()->GetPosition().m_x == (int)m_targetPosition.m_x &&
			(int)m_pOwner->GetOwner()->GetPosition().m_y == (int)m_targetPosition.m_y)
		{
			m_pOwner->ChangeToState(AIStateMachine::StateName::Return);
		}
	}
}
