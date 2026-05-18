#pragma once
#include "Core/State.h"

class LogState : public State
{
public:
    explicit LogState(Context& context);

    void Enter()                          override;
    void HandleInput(InputManager& input)  override;
    void Update()                         override;
    void Render(ConsoleDisplay& display)   override;

private:
    void Rebuild();

    int scrollOffset = 0;

    static constexpr int EntryH      = 3;   // 엔트리 1개 높이 (행)
    static constexpr int ListStartY  = 8;
    static constexpr int ListEndY    = 47;
    static constexpr int VisibleCount = (ListEndY - ListStartY) / EntryH;  // 13
};
