#define NOMINMAX
#include "WaitState.h"

#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UILabel.h"
#include "Ui/UIButton.h"

#include <string>
#include <algorithm>

namespace WaitLayout
{
    constexpr int Z       = 10;
    constexpr int RowH    = 3;

    constexpr int TitleY   = 13;
    constexpr int TimeY    = 19;
    constexpr int SelectorY = 25;

    // 화살표 + 라벨 레이아웃 (화면 폭 192 기준 중앙 배치)
    constexpr int ArrowW   = 8;
    constexpr int LabelW   = 30;
    constexpr int TotalW   = ArrowW + 2 + LabelW + 2 + ArrowW;  // 50
    constexpr int StartX   = (192 - TotalW) / 2;                // 71
    constexpr int LeftArrowX  = StartX;
    constexpr int LabelX      = StartX + ArrowW + 2;
    constexpr int RightArrowX = StartX + ArrowW + 2 + LabelW + 2;

    constexpr int ConfirmX = (192 - 50) / 2;
    constexpr int ConfirmW = 50;
    constexpr int ConfirmY = 33;
    constexpr int CancelY  = 38;
}

WaitState::WaitState(Context& context)
    : State(context)
{
}

void WaitState::Enter()
{
    Rebuild();
}

void WaitState::Rebuild()
{
    uiManager.Clear();

    // ── 제목 ─────────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, WaitLayout::TitleY, WaitLayout::Z,
        192, WaitLayout::RowH, L("ui.wait"),
        14, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 현재 시간 표시 ────────────────────────────────────────────────────────
    std::string timeStr = L("ui.wait_current_time") + " : "
        + "Day " + std::to_string(context.player.day)
        + "  T+" + std::to_string(context.player.time);

    uiManager.Add(std::make_unique<UILabel>(
        0, WaitLayout::TimeY, WaitLayout::Z,
        192, WaitLayout::RowH, timeStr,
        8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 시간 선택기 ───────────────────────────────────────────────────────────
    // ◄ 버튼
    bool canDecrease = (selectedHours > MinHours);
    if (canDecrease)
    {
        uiManager.Add(std::make_unique<UIButton>(
            WaitLayout::LeftArrowX, WaitLayout::SelectorY,
            WaitLayout::ArrowW, WaitLayout::RowH, WaitLayout::Z,
            "◄",
            [this]()
            {
                if (selectedHours > MinHours)
                {
                    --selectedHours;
                    Rebuild();
                }
            }));
    }
    else
    {
        uiManager.Add(std::make_unique<UILabel>(
            WaitLayout::LeftArrowX, WaitLayout::SelectorY, WaitLayout::Z,
            WaitLayout::ArrowW, WaitLayout::RowH, "◄",
            8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
    }

    // 시간 라벨
    std::string hourLabel = std::to_string(selectedHours) + " " + L("ui.wait_hour_unit");
    uiManager.Add(std::make_unique<UILabel>(
        WaitLayout::LabelX, WaitLayout::SelectorY, WaitLayout::Z,
        WaitLayout::LabelW, WaitLayout::RowH, hourLabel,
        14, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ► 버튼
    bool canIncrease = (selectedHours < MaxHours);
    if (canIncrease)
    {
        uiManager.Add(std::make_unique<UIButton>(
            WaitLayout::RightArrowX, WaitLayout::SelectorY,
            WaitLayout::ArrowW, WaitLayout::RowH, WaitLayout::Z,
            "►",
            [this]()
            {
                if (selectedHours < MaxHours)
                {
                    ++selectedHours;
                    Rebuild();
                }
            }));
    }
    else
    {
        uiManager.Add(std::make_unique<UILabel>(
            WaitLayout::RightArrowX, WaitLayout::SelectorY, WaitLayout::Z,
            WaitLayout::ArrowW, WaitLayout::RowH, "►",
            8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
    }

    // ── 확인 버튼 ─────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UIButton>(
        WaitLayout::ConfirmX, WaitLayout::ConfirmY,
        WaitLayout::ConfirmW, WaitLayout::RowH, WaitLayout::Z,
        L("ui.wait_confirm"),
        [this]() { Confirm(); }));

    // ── 취소 버튼 ─────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UIButton>(
        WaitLayout::ConfirmX, WaitLayout::CancelY,
        WaitLayout::ConfirmW, WaitLayout::RowH, WaitLayout::Z,
        L("ui.back"),
        [this]()
        {
            context.sound.PlaySE("Assets/audio/ui_button_click.wav");
            context.PopState();
        }));
}

void WaitState::Confirm()
{
    context.sound.PlaySE("Assets/audio/ui_button_click.wav");

    context.player.time += selectedHours;
    while (context.player.time >= 24)
    {
        context.player.time -= 24;
        context.player.day  += 1;
    }

    context.AddLog(L("log.waited")
        + " (+" + std::to_string(selectedHours) + "h"
        + " → Day " + std::to_string(context.player.day)
        + "  T+" + std::to_string(context.player.time) + ")");

    context.PopState();
}

void WaitState::HandleInput(InputManager& input)
{
    while (input.HasAction())
    {
        auto action = input.PopAction();
        switch (action)
        {
        case InputAction::MoveLeft:
            if (selectedHours > MinHours) { --selectedHours; Rebuild(); }
            break;
        case InputAction::MoveRight:
            if (selectedHours < MaxHours) { ++selectedHours; Rebuild(); }
            break;
        case InputAction::Confirm:
            Confirm();
            break;
        case InputAction::Cancel:
            context.PopState();
            break;
        default:
            break;
        }
    }
}

void WaitState::Update() {}

void WaitState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
