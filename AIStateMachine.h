#pragma once

class State;
class GameObject;

class AIStateMachine
{
public:
	enum class StateName
	{
		None,
		Pace,
		Chase,
		Return,
		Charge
	};
private:
	// Pointer to object that owns this state machine
	GameObject* m_pOwner;

	// Pointer to current target object
	GameObject* m_pTarget;

	// Currently active state
	State* m_pCurrentState;

	// Currently active state name
	StateName m_currentStateName;
public:
	// Constructor/Destructor
	AIStateMachine(GameObject* pOwner, GameObject* pTarget, StateName initialState);
	~AIStateMachine();

	// Updates the current state
	void Update(double deltaTime);

	// Changes states, won't change if passed state == current state
	void ChangeToState(StateName state);

	// Gets this state machine's owner
	GameObject* GetOwner() { return m_pOwner; }
	
	// Gets this state machine's current state
	StateName GetCurrentStateName() { return m_currentStateName; }
};


