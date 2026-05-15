#pragma once
#include "Core/State.h"

class SleepState : public State
{
public:
    explicit SleepState(Context& context);

    void Enter()                           override;
    void HandleInput(InputManager& input)  override;
    void Update()                          override;
    void Render(ConsoleDisplay& display)   override;
};
