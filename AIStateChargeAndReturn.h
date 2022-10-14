#pragma once
#include "AIStateMachine.h"
#include "GameObject.h"
#include "State.h"
#include "Vector2.h"

class AIStateChargeAndReturn :
    public State
{
private:
    // Pointer to state machine that owns this
    AIStateMachine* m_pOwner;

    // This objects target
    GameObject* m_pTarget;

    // Objects initial location to return to
    Vector2 m_targetPosition;

    // Seconds to stay still before charge
    double m_timer;

public:
    // Constructor/Destructor
    AIStateChargeAndReturn(AIStateMachine* pOwner, GameObject* pTarget);
    ~AIStateChargeAndReturn();

    // Updates this state
    virtual void Update(double deltaTime) override;
};

