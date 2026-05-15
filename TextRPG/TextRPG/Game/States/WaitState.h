#pragma once
#include "Core/State.h"

class WaitState : public State
{
public:
    explicit WaitState(Context& context);

    void Enter()                           override;
    void HandleInput(InputManager& input)  override;
    void Update()                          override;
    void Render(ConsoleDisplay& display)   override;

private:
    void Rebuild();
    void Confirm();

    int selectedHours = 1;

    static constexpr int MinHours = 1;
    static constexpr int MaxHours = 12;
};
