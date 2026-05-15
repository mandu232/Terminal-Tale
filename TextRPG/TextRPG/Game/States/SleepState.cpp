#define NOMINMAX
#include "SleepState.h"

#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UILabel.h"
#include "Ui/UIButton.h"

#include <string>
#include <algorithm>

namespace SleepLayout
{
    constexpr int Z       = 10;
    constexpr int PanelX  = 71;
    constexpr int PanelW  = 50;
    constexpr int RowH    = 3;

    constexpr int TitleY   = 11;
    constexpr int StatusY  = 17;
    constexpr int VitalityY = 21;
    constexpr int DescY    = 27;
    constexpr int ConfirmY = 33;
    constexpr int CancelY  = 38;

    constexpr int VitalityRestore = 50;
    constexpr int VitalityMax     = 100;
}

SleepState::SleepState(Context& context)
    : State(context)
{
}

void SleepState::Enter()
{
    // ── 제목 ─────────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, SleepLayout::TitleY, SleepLayout::Z,
        192, SleepLayout::RowH, L("ui.sleep"),
        14, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 현재 날짜/시간 ────────────────────────────────────────────────────────
    std::string statusStr =
        "Day " + std::to_string(context.player.day)
        + "  " + std::to_string(context.player.time) + "시";

    uiManager.Add(std::make_unique<UILabel>(
        0, SleepLayout::StatusY, SleepLayout::Z,
        192, SleepLayout::RowH, statusStr,
        8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 현재 체력 ─────────────────────────────────────────────────────────────
    int afterVitality = std::min(
        context.player.vitality + SleepLayout::VitalityRestore,
        SleepLayout::VitalityMax);

    std::string vitalStr =
        L("stat.vitality") + " : "
        + std::to_string(context.player.vitality)
        + "  →  " + std::to_string(afterVitality);

    uiManager.Add(std::make_unique<UILabel>(
        0, SleepLayout::VitalityY, SleepLayout::Z,
        192, SleepLayout::RowH, vitalStr,
        7, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 설명 ─────────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, SleepLayout::DescY, SleepLayout::Z,
        192, SleepLayout::RowH, L("ui.sleep_desc"),
        8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 수면 확인 버튼 ────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UIButton>(
        SleepLayout::PanelX, SleepLayout::ConfirmY,
        SleepLayout::PanelW, SleepLayout::RowH, SleepLayout::Z,
        L("ui.sleep_confirm"),
        [this]()
        {
            context.sound.PlaySE("Assets/audio/ui_button_click.wav");

            context.player.day   += 1;
            context.player.time   = 0;
            context.player.vitality = std::min(
                context.player.vitality + SleepLayout::VitalityRestore,
                SleepLayout::VitalityMax);

            context.AddLog(L("log.slept")
                + " → Day " + std::to_string(context.player.day));

            context.PopState();
        }));

    // ── 취소 버튼 ─────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UIButton>(
        SleepLayout::PanelX, SleepLayout::CancelY,
        SleepLayout::PanelW, SleepLayout::RowH, SleepLayout::Z,
        L("ui.back"),
        [this]()
        {
            context.sound.PlaySE("Assets/audio/ui_button_click.wav");
            context.PopState();
        }));
}

void SleepState::HandleInput(InputManager& input)
{
    while (input.HasAction())
    {
        auto action = input.PopAction();
        if (action == InputAction::Cancel)
            context.PopState();
    }
}

void SleepState::Update() {}

void SleepState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
