#pragma once
#include <vector>

class ColliderComponent;

///////////////////////////////////////////////////////////////////
// Maintains references to all colliders and check their collisions
class CollisionReferee
{
private:
	// All currently active colliders
	std::vector<ColliderComponent*> m_activeColliders;

public:
	CollisionReferee();
	~CollisionReferee();

	// Register a new collider
	void AddActiveCollider(ColliderComponent* pCollider);

	// Unregister a collider
	void RemoveActiveCollider(ColliderComponent* pCollider);

	// Find all colliders that overlap and notify them of collision via bool return
	int CheckForCollisionAndNotify(ColliderComponent* pColliderToCheck);
};

