#pragma once
#include "GameState.h"
#include "ImageObject.h"
#include "UIButton.h"
#include "UITextField.h"

class GameObject;
class CatGameStateMachine;

class StateLoad :
    public GameState
{
private:
    // State machine that is operating this state
    CatGameStateMachine* m_pOwner;

    // Vector of game objects
    std::vector<GameObject*> m_gameObjects;

    // Vector of Buttons
    std::vector<UIButton*> m_buttonSet;

    // Vector of traversable Buttons
    std::vector<UIButton*> m_traversableButtonSet;

    // Array of if buttons are loadable
    bool m_loadable[3];

    // Current highlighted button index
    int m_keyboardButtonIndex;

    // Image Handler
    ImageHandler* m_pImageHandler;

public:
    // Constructor/Destructor
    StateLoad(CatGameStateMachine* pOwner, TTF_Font* pFont);
    ~StateLoad();

    // Renders this scene
    virtual void Render(SDL_Renderer* pRenderer) override;

    // Updates this scene
    virtual void Update(double deltaTime) override;

    // Handles current event, returns true if quit
    virtual bool HandleEvent(SDL_Event* pEvent) override;

private:
    // Changes button focus when using keyboard
    void ChangeButtonFocus(int direction);
};

