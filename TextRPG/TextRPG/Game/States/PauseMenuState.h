#pragma once

#include "Core/State.h"

class PauseMenuState : public State
{
public:
    explicit PauseMenuState(Context& context);
    void Enter()                              override;
    void HandleInput(InputManager& input)     override;
    void Update()                             override;
    void Render(ConsoleDisplay& display)      override;

private:
    void Rebuild();
};
