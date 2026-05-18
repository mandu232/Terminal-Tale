#pragma once
#include "Core/State.h"

class AchievementState : public State
{
public:
    explicit AchievementState(Context& context);

    void Enter()                          override;
    void Exit()                           override {}
    void HandleInput(InputManager& input)  override;
    void Update()                         override {}
    void Render(ConsoleDisplay& display)   override;

private:
    void Rebuild();

    int scrollOffset = 0;

    static constexpr int ListStartY   = 8;
    static constexpr int EntryH       = 4;   // 이름(1) + 설명(2) + 간격(1)
    static constexpr int ListEndY     = 48;
    static constexpr int VisibleCount = (ListEndY - ListStartY) / EntryH; // 10
};
