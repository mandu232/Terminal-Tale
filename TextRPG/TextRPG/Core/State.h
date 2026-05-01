#pragma once
#include "UIManager.h"

class InputManager;
class Context;
class ConsoleDisplay;

class State
{
public:
    State(Context& context)
        : context(context) {
    }

    virtual ~State() = default;

    virtual void Enter() {}
    virtual void Exit() {}
	virtual void Resume() {}

    virtual void HandleInput(InputManager& input) = 0;
    virtual void Update() = 0;
    virtual void Render(ConsoleDisplay& display) = 0;

	UIManager& GetUIManager() { return uiManager; }

protected:
    Context& context;
	UIManager uiManager;
};