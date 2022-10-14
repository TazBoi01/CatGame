#include "ColliderComponent.h"
#include "CollisionReferee.h"

ColliderComponent::ColliderComponent(GameObject* pOwner, SDL_FRect transform, CollisionReferee* pReferee, bool isTrigger)
	: m_pOwner(pOwner)
	, m_position{ (float)transform.x, (float)transform.y }
	, m_size{ (float)transform.w, (float)transform.h }
	, m_pReferee(pReferee)
	, m_isTrigger(isTrigger)
	, m_collisionType(0)
{
	if (pReferee != nullptr)
	{
		pReferee->AddActiveCollider(this);
	}

}

ColliderComponent::~ColliderComponent()
{
	if (m_pReferee != nullptr)
	{
		m_pReferee->RemoveActiveCollider(this);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Attempt to move by given amount, returns whether move was successful or not
bool ColliderComponent::TryMove(Vector2 deltaPosition, double speed, double deltaTime)
{

	// Make sure there's a referee assigned
	if (m_pReferee == nullptr)
	{
		return true;
	}

	// Determine movement amount of x with delta time, updataes position, and then pass m_x to m_transform
	double deltaPositionX = speed * deltaTime;
	m_position.m_x += (float)deltaPositionX * deltaPosition.m_x;

	// Determine movement amount of y with delta time, updataes position, and then pass m_y to m_transform
	double deltaPositionY = speed * deltaTime;
	m_position.m_y += (float)deltaPositionY * deltaPosition.m_y;

	// Perform the collision check
	int didCollide = m_pReferee->CheckForCollisionAndNotify(this);
	// If the move was invalid, undo it

	if (didCollide == 1)
	{
		m_position.m_x -= (float)deltaPositionX * deltaPosition.m_x;
		m_position.m_y -= (float)deltaPositionY * deltaPosition.m_y;
	}

	// Bool and check for successful move
	bool successfulMove = true;
	if (didCollide == 1)
		successfulMove = false;
	
	// Return whether the move was successful or not
	return successfulMove;
}
