#pragma once

class State
{
public:
	virtual ~State() {}

	// Enters this state
	virtual void Enter() {}

	// Updates this state
	virtual void Update(double) {}

	// Exits this state
	virtual void Exit() {}
};

