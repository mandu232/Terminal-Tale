#include "LoadSlotState.h"
#include "StoryState.h"
#include "InventoryState.h"

#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UIButton.h"
#include "Ui/UILabel.h"

#include <string>

namespace LoadSlotLayout
{
    constexpr int Z      = 10;
    constexpr int SlotW  = 50;
    constexpr int SlotH  = 5;
    constexpr int SlotX  = (192 - SlotW) / 2;
    constexpr int Slot1Y = 14;
    constexpr int SlotGap = SlotH + 3;

    constexpr int BackW  = 25;
    constexpr int BackX  = (192 - BackW) / 2;
    constexpr int BackY  = 42;
}

LoadSlotState::LoadSlotState(Context& context)
    : State(context)
{
}

void LoadSlotState::Enter()
{
    // ── 타이틀 ───────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, 5, LoadSlotLayout::Z, 192, 3,
        L("ui.load_game"),
        14, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 안내 ─────────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, 9, LoadSlotLayout::Z, 192, 2,
        L("ui.load_slot_hint"),
        8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 슬롯 3개 ─────────────────────────────────────────────────────────────
    for (int i = 1; i <= 3; ++i)
    {
        int y = LoadSlotLayout::Slot1Y + (i - 1) * LoadSlotLayout::SlotGap;
        auto info = context.GetSlotInfo(i);

        std::string label = L("ui.slot") + " " + std::to_string(i) + "  —  ";

        if (info.exists)
        {
            label += "Day " + std::to_string(info.day)
                   + "  T+" + std::to_string(info.time);

            int slotIndex = i;
            uiManager.Add(std::make_unique<UIButton>(
                LoadSlotLayout::SlotX, y,
                LoadSlotLayout::SlotW, LoadSlotLayout::SlotH, LoadSlotLayout::Z,
                label,
                [this, slotIndex]()
                {
                    context.sound.PlaySE("Assets/audio/ui_button_click.wav");
                    context.activeSlot = slotIndex;
                    context.LoadSlot(slotIndex);

                    auto story = std::make_unique<StoryState>(context, context.currentNodeId);
                    story->onInventory = [this]()
                    {
                        context.PushState(std::make_unique<InventoryState>(context));
                    };
                    context.ChangeState(std::move(story));
                }));
        }
        else
        {
            label += L("ui.slot_no_data");
            uiManager.Add(std::make_unique<UILabel>(
                LoadSlotLayout::SlotX, y, LoadSlotLayout::Z,
                LoadSlotLayout::SlotW, LoadSlotLayout::SlotH,
                label,
                8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
        }
    }

    // ── 뒤로 버튼 ─────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UIButton>(
        LoadSlotLayout::BackX, LoadSlotLayout::BackY,
        LoadSlotLayout::BackW, 3, LoadSlotLayout::Z,
        L("ui.back"),
        [this]()
        {
            context.sound.PlaySE("Assets/audio/ui_button_click.wav");
            context.PopState();
        }));
}

void LoadSlotState::HandleInput(InputManager& input)
{
    while (input.HasAction())
    {
        auto action = input.PopAction();
        if (action == InputAction::Cancel)
            context.PopState();
    }
}

void LoadSlotState::Update() {}

void LoadSlotState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
