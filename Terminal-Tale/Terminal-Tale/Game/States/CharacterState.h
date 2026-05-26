#pragma once
#include "Core/State.h"

class CharacterState : public State
{
public:
    explicit CharacterState(Context& context);

    void Enter()                          override;
    void HandleInput(InputManager& input)  override;
    void Update()                         override;
    void Render(ConsoleDisplay& display)   override;

private:
    void Rebuild();

    int selectedIndex = -1;
};
