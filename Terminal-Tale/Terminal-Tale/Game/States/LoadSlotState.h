#pragma once

#include "Core/State.h"

class LoadSlotState : public State
{
public:
    explicit LoadSlotState(Context& context);

    void Enter()                           override;
    void HandleInput(InputManager& input)  override;
    void Update()                          override;
    void Render(ConsoleDisplay& display)   override;
};
