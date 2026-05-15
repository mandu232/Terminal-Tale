#define NOMINMAX
#include "JournalState.h"

#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UILabel.h"
#include "Ui/UIButton.h"

#include <string>

namespace JnLayout
{
    constexpr int Z        = 10;
    constexpr int RowH     = 3;

    // 전체 제목
    constexpr int TitleY   = 1;

    // 구분선 (세로 x=62)
    constexpr int DivX     = 62;

    // ── 좌측: 저널 목록 ──────────────────────────────────────────────────────
    constexpr int ListX    = 2;
    constexpr int ListW    = 58;
    constexpr int ListStartY = 7;
    constexpr int MaxItems   = 13;   // (46 - 7) / 3

    // ── 우측: 선택된 저널 내용 ────────────────────────────────────────────────
    constexpr int RightX   = 65;
    constexpr int RightW   = 124;
    constexpr int EntryTitleY   = 7;
    constexpr int EntryDayY     = 11;
    constexpr int EntryContentY = 15;
    constexpr int EntryContentH = 32;

    // 닫기 버튼
    constexpr int CloseX   = 83;
    constexpr int CloseW   = 25;
    constexpr int CloseY   = 50;
}

JournalState::JournalState(Context& context)
    : State(context)
{
}

void JournalState::Enter()
{
    selectedIndex = context.journal.empty() ? -1 : 0;
    Rebuild();
}

// Enter 에서 처음 호출, 항목 선택 시 재호출
void JournalState::Rebuild()
{
    uiManager.Clear();
    contentTitleLabel = contentDayLabel = contentBodyLabel = nullptr;

    // ── 전체 제목 ─────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, JnLayout::TitleY, JnLayout::Z,
        192, JnLayout::RowH, L("ui.journal"),
        14, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 세로 구분선 ───────────────────────────────────────────────────────────
    for (int row = 5; row < 50; ++row)
    {
        uiManager.Add(std::make_unique<UILabel>(
            JnLayout::DivX, row, JnLayout::Z,
            2, 1, "|",
            8, UILabel::TextAlign::Left, UILabel::VAlign::Top));
    }

    // ── 좌측: 목록 제목 ───────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        JnLayout::ListX, 5, JnLayout::Z,
        JnLayout::ListW, JnLayout::RowH, L("ui.journal_list"),
        8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    if (context.journal.empty())
    {
        uiManager.Add(std::make_unique<UILabel>(
            JnLayout::ListX, JnLayout::ListStartY, JnLayout::Z,
            JnLayout::ListW, JnLayout::RowH, L("ui.journal_empty"),
            8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
    }
    else
    {
        // 저널 항목 버튼 목록
        int y = JnLayout::ListStartY;
        int count = std::min(static_cast< int >( context.journal.size() ) , JnLayout::MaxItems);

        for (int i = 0; i < count; ++i)
        {
            const auto& entry = context.journal[i];
            bool isSelected = (i == selectedIndex);
            int idx = i;

            auto btn = std::make_unique<UIButton>(
                JnLayout::ListX, y,
                JnLayout::ListW, JnLayout::RowH, JnLayout::Z,
                entry.title,
                [this, idx]()
                {
                    context.sound.PlaySE("Assets/audio/ui_button_click.wav");
                    selectedIndex = idx;
                    Rebuild();
                });

            uiManager.Add(std::move(btn));
            y += JnLayout::RowH;
        }
    }

    // ── 우측: 선택된 저널 내용 ────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        JnLayout::RightX, 5, JnLayout::Z,
        JnLayout::RightW, JnLayout::RowH, L("ui.journal_detail"),
        8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(context.journal.size()))
    {
        const auto& entry = context.journal[selectedIndex];

        // 제목
        {
            auto lbl = std::make_unique<UILabel>(
                JnLayout::RightX, JnLayout::EntryTitleY, JnLayout::Z,
                JnLayout::RightW, JnLayout::RowH, entry.title,
                14, UILabel::TextAlign::Left, UILabel::VAlign::Middle);
            contentTitleLabel = lbl.get();
            uiManager.Add(std::move(lbl));
        }

        // 날짜
        {
            std::string dayStr = "Day " + std::to_string(entry.day);
            auto lbl = std::make_unique<UILabel>(
                JnLayout::RightX, JnLayout::EntryDayY, JnLayout::Z,
                JnLayout::RightW, JnLayout::RowH, dayStr,
                8, UILabel::TextAlign::Left, UILabel::VAlign::Middle);
            contentDayLabel = lbl.get();
            uiManager.Add(std::move(lbl));
        }

        // 본문
        {
            auto lbl = std::make_unique<UILabel>(
                JnLayout::RightX, JnLayout::EntryContentY, JnLayout::Z,
                JnLayout::RightW, JnLayout::EntryContentH, entry.content,
                7, UILabel::TextAlign::Left, UILabel::VAlign::Top);
            contentBodyLabel = lbl.get();
            uiManager.Add(std::move(lbl));
        }
    }
    else if (!context.journal.empty())
    {
        // 항목은 있지만 아무것도 선택 안 된 경우 (이론상 없음)
        uiManager.Add(std::make_unique<UILabel>(
            JnLayout::RightX, 25, JnLayout::Z,
            JnLayout::RightW, JnLayout::RowH, L("ui.journal_select_hint"),
            8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
    }

    // ── 닫기 버튼 ─────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UIButton>(
        JnLayout::CloseX, JnLayout::CloseY,
        JnLayout::CloseW, JnLayout::RowH, JnLayout::Z,
        L("ui.close"),
        [this]()
        {
            context.sound.PlaySE("Assets/audio/ui_button_click.wav");
            context.PopState();
        }));
}

void JournalState::HandleInput(InputManager& input)
{
    while (input.HasAction())
    {
        auto action = input.PopAction();
        if (action == InputAction::Cancel)
            context.PopState();
    }
}

void JournalState::Update() {}

void JournalState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
