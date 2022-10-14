#pragma once
#include <SDL.h>
#include "Vector2.h"

class GameObject;
class CollisionReferee;

////////////////////////////////////////
// An anti-aligned bounding box collider
class ColliderComponent
{
private:
	// Pointer to the GameObject that owns collider
	GameObject* m_pOwner;

	// Position and dimensions
	Vector2 m_position;
	Vector2 m_size;

	// Pointer to referee for registering and collision checking
	CollisionReferee* m_pReferee;

	// Bool to store if item is a trigger
	bool m_isTrigger;

	// Int to store collision type
	int m_collisionType = 0;

public:
	ColliderComponent(GameObject* pOwner, SDL_FRect transform, CollisionReferee* pReferee, bool isTrigger);
	~ColliderComponent();

	// Returns pointer to this collider's owner
	GameObject* GetOwner() { return m_pOwner; }

	// Returns position and dimensions
	Vector2 GetPosition() { return m_position; }
	
	// Returns dimensions
	Vector2 GetSize() { return m_size; }

	// Returns if is trigger
	bool GetIsTrigger() { return m_isTrigger; }

	// Returns if collided last frame
	int GetCollisionType() { return m_collisionType; }

	// Updates Position
	void SetPosition(Vector2 newPosition) { m_position = newPosition; }

	// Updates Size
	void SetSize(Vector2 newSize) { m_size = newSize; }

	// Returns if collided last frame
	void SetCollisionType(int newCollisionType) { m_collisionType = newCollisionType; }

	// Attempt to move by given amount, returns whether move was successful or not
	bool TryMove(Vector2 deltaPositon, double speed, double deltaTime);
};

