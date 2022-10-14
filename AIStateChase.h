#pragma once
#include "GameObject.h"
#include "State.h"

class AIStateMachine;

class AIStateChase :
    public State
{
private:
    // Pointer to state machine that owns this
    AIStateMachine* m_pOwner;

    // Object to chase
    GameObject* m_pTarget;
    
public:
    // Constructor/Destructor
    AIStateChase(AIStateMachine* pOwner, GameObject* pTarget);
    ~AIStateChase();

    // Updates this state
    virtual void Update(double deltaTime) override;
};

