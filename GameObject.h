#pragma once
#include "Vector2.h"

#include <SDL.h>
#include <string>

class ColliderComponent;

class GameObject
{
public:
	// Destructor
	virtual ~GameObject() {}

	// Returns a readable name
	virtual std::string GetName() { return "Unnamed"; }

	// Gets called by the main loop each frame to update this object's state
	virtual void Update(double) {}

	// Gets called by the main loop each frame to render this object
	virtual void Render(SDL_Renderer*) {}
	
	// When an item first collides with another
	virtual void OnCollisionEntry(ColliderComponent*) {}

	// For continuous update
	virtual void OnCollisionUpdate(ColliderComponent*) {}

	// When an item first collides with another
	virtual void OnCollisionExit(ColliderComponent*) {}

	// Object handles given event
	virtual void HandleEvent(SDL_Event*) {}

	// Attempts to move this object
	virtual bool Move(Vector2, double, float = 1) { return true; }

	// Gets this objects position
	virtual Vector2 GetPosition() { return Vector2{ 0, 0 }; }
};

