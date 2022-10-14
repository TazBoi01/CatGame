#include "AIStateMachine.h"
#include "AIStatePace.h"
#include "GameObject.h"

AIStatePace::AIStatePace(AIStateMachine* pOwner, Vector2 pacingBounds, int axis)
	: m_pOwner(pOwner)
	, m_pacingBounds(pacingBounds)
	, m_axis(axis)
	, m_direction(1)
{
}

AIStatePace::~AIStatePace()
{
}

void AIStatePace::Update(double deltaTime)
{
	// If x-axis, then go towards right bounds
	if (m_axis == 0)
	{
		// If moving right and pos is greater than bounds
		if (m_direction == 1 && m_pOwner->GetOwner()->GetPosition().m_x < m_pacingBounds.m_y)
		{
			// Make path the outer bounds for x and the owners y pos
			Vector2 pathToTarget = Vector2{ m_pacingBounds.m_y, m_pOwner->GetOwner()->GetPosition().m_y } - m_pOwner->GetOwner()->GetPosition();
			pathToTarget = pathToTarget.GetNormalized();
			m_pOwner->GetOwner()->Move(pathToTarget, deltaTime);
		}
		else
			m_direction = 0;
		if (m_direction == 0 && m_pOwner->GetOwner()->GetPosition().m_x > m_pacingBounds.m_x)
		{
			Vector2 pathToTarget = Vector2{ m_pacingBounds.m_x, m_pOwner->GetOwner()->GetPosition().m_y } - m_pOwner->GetOwner()->GetPosition();
			pathToTarget = pathToTarget.GetNormalized();
			m_pOwner->GetOwner()->Move(pathToTarget, deltaTime);
		}
		else 
			m_direction = 1;
	}
	if (m_axis == 1)
	{
		// If moving up and pos is greater than bounds
		if (m_direction == 1 && m_pOwner->GetOwner()->GetPosition().m_y < m_pacingBounds.m_y)
		{
			Vector2 pathToTarget = Vector2{ m_pOwner->GetOwner()->GetPosition().m_x, m_pacingBounds.m_y } - m_pOwner->GetOwner()->GetPosition();
			pathToTarget = pathToTarget.GetNormalized();
			m_pOwner->GetOwner()->Move(pathToTarget, deltaTime);
		}
		else if (m_direction == 0 && m_pOwner->GetOwner()->GetPosition().m_y > m_pacingBounds.m_x)
		{
			Vector2 pathToTarget = Vector2{ m_pOwner->GetOwner()->GetPosition().m_x, m_pacingBounds.m_x } - m_pOwner->GetOwner()->GetPosition();
			pathToTarget = pathToTarget.GetNormalized();
			m_pOwner->GetOwner()->Move(pathToTarget, deltaTime);
		}
	}
}
