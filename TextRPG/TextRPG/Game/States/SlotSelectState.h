#pragma once

#include "Core/State.h"
#include <functional>

class SlotSelectState : public State
{
public:
    SlotSelectState(Context& context,
        std::function<void(int slotIndex)> onSlotSelected);

    void Enter() override;
    void HandleInput(InputManager& input) override;
    void Update() override;
    void Render(ConsoleDisplay& display) override;

private:
    std::function<void(int slotIndex)> onSlotSelected;
};
