#include "CollisionReferee.h"
#include "ColliderComponent.h"
#include "GameObject.h"

CollisionReferee::CollisionReferee()
{
}

CollisionReferee::~CollisionReferee()
{
}

//////////////////////////
// Register a new collider
void CollisionReferee::AddActiveCollider(ColliderComponent* pCollider)
{
	// Make sure it isn't already present
	if (std::find(m_activeColliders.begin(), m_activeColliders.end(), pCollider) == m_activeColliders.end())
	{
		// If not, add it to the vector
		m_activeColliders.push_back(pCollider);
	}
}

////////////////////////
// Unregister a collider
void CollisionReferee::RemoveActiveCollider(ColliderComponent* pCollider)
{
	//Find the collider.
	auto location = std::find(m_activeColliders.begin(), m_activeColliders.end(), pCollider);
		
	// If found, erase it from the vector
	if (location != m_activeColliders.end())
	{
		m_activeColliders.erase(location);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Find all colliders that overlap and notify them of collision via bool return
int CollisionReferee::CheckForCollisionAndNotify(ColliderComponent* pColliderToCheck)
{
	// Constexpr for collision types
	constexpr int kNoCollide = 0;
	constexpr int kDidCollide = 1;
	constexpr int kDidCollideTrigger = 2;

	// Allocate a bool for if any collisions occurred
	int collisionState = kNoCollide;

	// Get pColliderToCheck's transform
	Vector2 colliderPosition = pColliderToCheck->GetPosition();
	Vector2 colliderSize = pColliderToCheck->GetSize();

	// Calculate bounds
	float left = colliderPosition.m_x;
	float right = left + colliderSize.m_x;
	float top = colliderPosition.m_y;
	float bottom = top + colliderSize.m_y;

	// Check against every active collider
	for(ColliderComponent* pOtherCollider : m_activeColliders)
	{
		// Make sure we don't check a collider vs itself
		if (pColliderToCheck == pOtherCollider)
			continue;
		// Get other collider's transform
		Vector2 otherColliderPosition = pOtherCollider->GetPosition();
		Vector2 otherColliderSize = pOtherCollider->GetSize();

		// Calculate the bounds of the other collider
		float otherLeft = otherColliderPosition.m_x;
		float otherRight = otherLeft + otherColliderSize.m_x;
		float otherTop = otherColliderPosition.m_y;
		float otherBottom = otherTop + otherColliderSize.m_y;

		// Check if they overlap horizontally
		bool xOverlap = left < otherRight && right > otherLeft;

		// Check if they overlap vertically
		bool yOverlap = top < otherBottom && bottom > otherTop;

		// If both x and y are true, they collide
		if(xOverlap && yOverlap)
		{
			// If the item connects with a trigger
			if (pOtherCollider->GetIsTrigger())
			{
				// If weren't colliding last frame, on collision entry
				if (pColliderToCheck->GetCollisionType() == 0 && pOtherCollider->GetCollisionType() == 0)
				{
					pColliderToCheck->GetOwner()->OnCollisionEntry(pOtherCollider);
					pOtherCollider->GetOwner()->OnCollisionEntry(pColliderToCheck);
				}
				collisionState = kDidCollideTrigger;
				// Set collisionState for this frame
				pColliderToCheck->SetCollisionType(collisionState);
				pOtherCollider->SetCollisionType(collisionState);
			}
			else
			{ 
				// A collision has occurred
				collisionState = kDidCollide;
				// Set collisionState for this frame
				pColliderToCheck->SetCollisionType(collisionState);
				pOtherCollider->SetCollisionType(collisionState);
			}
			// Notify both colliders' owners of the collision
			pColliderToCheck->GetOwner()->OnCollisionUpdate(pOtherCollider); // notify object A
			pOtherCollider->GetOwner()->OnCollisionUpdate(pColliderToCheck); // notify object B
		}
		else
		{
			// If were colliding last frame, but no overlap, on collision exit
			if (pColliderToCheck->GetCollisionType() == 2 && pOtherCollider->GetCollisionType() == 2)
			{
				pColliderToCheck->GetOwner()->OnCollisionExit(pOtherCollider);
				pOtherCollider->GetOwner()->OnCollisionExit(pColliderToCheck);
			collisionState = kNoCollide;
			// Set collisionState for this frame
			pColliderToCheck->SetCollisionType(collisionState);
			pOtherCollider->SetCollisionType(collisionState);
			}
		}
	}

	// Return whether any collisions occured
	return collisionState;
}