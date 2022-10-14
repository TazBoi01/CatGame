#include "AIStateChase.h"
#include "AIStateMachine.h"
#include "Vector2.h"

AIStateChase::AIStateChase(AIStateMachine* pOwner, GameObject* pTarget)
	: m_pOwner(pOwner)
	, m_pTarget(pTarget)
{
}

AIStateChase::~AIStateChase()
{
}

/////////////////////
// Updates this state
void AIStateChase::Update(double deltaTime)
{
	Vector2 pathToTarget = m_pTarget->GetPosition() - m_pOwner->GetOwner()->GetPosition();
	pathToTarget = pathToTarget.GetNormalized();
	m_pOwner->GetOwner()->Move(pathToTarget, deltaTime);
}
