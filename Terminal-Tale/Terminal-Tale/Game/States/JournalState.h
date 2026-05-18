#pragma once
#include "Core/State.h"
#include <string>

class UILabel;

class JournalState : public State
{
public:
    explicit JournalState(Context& context);

    void Enter()                          override;
    void HandleInput(InputManager& input)  override;
    void Update()                         override;
    void Render(ConsoleDisplay& display)   override;

private:
    void Rebuild();

    int selectedIndex = 0;  // 현재 선택된 저널 항목 인덱스

    UILabel* contentTitleLabel   = nullptr;
    UILabel* contentDayLabel     = nullptr;
    UILabel* contentBodyLabel    = nullptr;
};
