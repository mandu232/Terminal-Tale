#include "SlotSelectState.h"
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UIButton.h"
#include "Ui/UILabel.h"

namespace
{
    constexpr int Z         = 10;

    // 화면 중앙 기준 (192×54)
    constexpr int SlotW     = 50;
    constexpr int SlotH     = 5;
    constexpr int SlotX     = (192 - SlotW) / 2;   // 71
    constexpr int Slot1Y    = 14;
    constexpr int Slot2Y    = 22;
    constexpr int Slot3Y    = 30;

    constexpr int BackW     = 25;
    constexpr int BackX     = (192 - BackW) / 2;    // 83
    constexpr int BackY     = 42;
}

SlotSelectState::SlotSelectState(Context& context,
    std::function<void(int)> onSlotSelected)
    : State(context)
    , onSlotSelected(std::move(onSlotSelected))
{
}

void SlotSelectState::Enter()
{
    // ── 타이틀 라벨 ───────────────────────────────────────────────────────────
    uiManager.Add(
        std::make_unique<UILabel>(
            0, 5, Z, 192, 3,
            L("ui.slot_select"),
            14,
            UILabel::TextAlign::Center,
            UILabel::VAlign::Middle)
    );

    // ── 안내 라벨 ─────────────────────────────────────────────────────────────
    uiManager.Add(
        std::make_unique<UILabel>(
            0, 9, Z, 192, 2,
            L("ui.slot_select_hint"),
            8,
            UILabel::TextAlign::Center,
            UILabel::VAlign::Middle)
    );

    // ── 슬롯 버튼 3개 ─────────────────────────────────────────────────────────
    for (int i = 1; i <= 3; ++i)
    {
        int y = Slot1Y + (i - 1) * (SlotH + 3);
        int slotIndex = i;

        auto info = context.GetSlotInfo(i);
        std::string label = L("ui.slot") + " " + std::to_string(i) + "  —  ";
        if (info.exists)
            label += "Day " + std::to_string(info.day)
                   + "  T+" + std::to_string(info.time);
        else
            label += L("ui.slot_empty");

        uiManager.Add(
            std::make_unique<UIButton>(
                SlotX, y, SlotW, SlotH, Z,
                label,
                [this, slotIndex]()
                {
                    context.sound.PlaySE("Assets/audio/ui_button_click.wav");
                    context.activeSlot = slotIndex;
                    if (onSlotSelected) onSlotSelected(slotIndex);
                })
        );
    }

    // ── 뒤로 버튼 ─────────────────────────────────────────────────────────────
    uiManager.Add(
        std::make_unique<UIButton>(
            BackX, BackY, BackW, 3, Z,
            L("ui.back"),
            [this]()
            {
                context.sound.PlaySE("Assets/audio/ui_button_click.wav");
                context.PopState();
            })
    );
}

void SlotSelectState::HandleInput(InputManager& input)
{
    while (input.HasAction())
    {
        auto action = input.PopAction();
        if (action == InputAction::Quit)
            context.PopState();
    }
}

void SlotSelectState::Update() {}

void SlotSelectState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
