#pragma once
#include "State.h"
#include "Vector2.h"

class AIStateReturn :
    public State
{
private:
    // Pointer to state machine that owns this
    AIStateMachine* m_pOwner;

    // Objects initial location to return to
    Vector2 m_homePosition;

public:
    // Constructor/Destructor
    AIStateReturn(AIStateMachine* pOwner, Vector2 homePosition);
    ~AIStateReturn();

    // Updates this state
    virtual void Update(double deltaTime) override;
};

