#pragma once
#include "State.h"
#include "Vector2.h"

class AIStatePace :
    public State
{
private:
    // Bounds of pacing on axis
    Vector2 m_pacingBounds;

    // 0 for X-Axis, 1 for Y-Axis
    int m_axis;

    // Pointer to state machine that owns this
    AIStateMachine* m_pOwner;

    // Direction to move: 0 for left, 1 for right
    int m_direction;

public:
    // Constructor/Destructor
    AIStatePace(AIStateMachine* pOwner, Vector2 pacingBounds, int axis);
    ~AIStatePace();

    // Updates this state
    virtual void Update(double deltaTime) override;
};

