#pragma once
#include "GameState.h"
#include "ImageObject.h"
#include "UIButton.h"
#include "UITextField.h"

class GameObject;
class CatGameStateMachine;

class StateMainMenu :
    public GameState
{
private:
    // State machine that is operating this state
    CatGameStateMachine* m_pOwner;

    // Vector of game objects
    std::vector<GameObject*> m_gameObjects;

    // Vector of Buttons
    std::vector<UIButton*> m_buttonSet;

    // Current highlighted button index
    int m_keyboardButtonIndex;

    // Image Handler
    ImageHandler* m_pImageHandler;

public:
    // Constructor/Destructor
    StateMainMenu(CatGameStateMachine* pOwner, TTF_Font* pFont);
    ~StateMainMenu();

    // Renders this scene
    virtual void Render(SDL_Renderer* pRenderer) override;

    // Handles current event, returns true if quit
    virtual bool HandleEvent(SDL_Event* pEvent) override;

private:
    // Changes button focus when using keyboard
    void ChangeButtonFocus(int direction);
};

