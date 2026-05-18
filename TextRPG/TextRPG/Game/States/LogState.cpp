#define NOMINMAX
#include "LogState.h"

#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UILabel.h"
#include "Ui/UIButton.h"

#include <string>
#include <algorithm>

namespace LogLayout
{
    constexpr int Z         = 10;
    constexpr int RowH      = 3;

    constexpr int TitleY    = 1;
    constexpr int DividerY  = 5;
    constexpr int ListX     = 4;
    constexpr int ListW     = 172;
    constexpr int TimeW     = 20;
    constexpr int TextX     = ListX + TimeW + 2;
    constexpr int TextW     = ListW - TimeW - 2;

    constexpr int EntryH    = 3;
    constexpr int ListStartY = 8;

    constexpr int ScrollX   = 180;
    constexpr int ScrollUpY = 8;
    constexpr int ScrollDnY = 45;

    constexpr int CloseX    = 83;
    constexpr int CloseW    = 25;
    constexpr int CloseY    = 50;
}

LogState::LogState(Context& context)
    : State(context)
{
}

void LogState::Enter()
{
    scrollOffset = 0;   // 0 = 가장 최신 항목부터
    Rebuild();
}

void LogState::Rebuild()
{
    uiManager.Clear();

    // ── 제목 ─────────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, LogLayout::TitleY, LogLayout::Z,
        192, LogLayout::RowH, L("ui.log"),
        14, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 구분선 ───────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, LogLayout::DividerY, LogLayout::Z,
        192, 1, std::string(192, '-'),
        8, UILabel::TextAlign::Left, UILabel::VAlign::Top));

    // ── 로그가 비어있을 때 ────────────────────────────────────────────────────
    const auto& log = context.log;
    if (log.empty())
    {
        uiManager.Add(std::make_unique<UILabel>(
            0, 25, LogLayout::Z,
            192, LogLayout::RowH, L("ui.log_empty"),
            8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
    }
    else
    {
        // ── 엔트리 목록 (최신순 = 역순) ──────────────────────────────────────
        int total  = static_cast<int>(log.size());
        int startI = total - 1 - scrollOffset;           // 역순 시작 인덱스
        int endI   = std::max(startI - VisibleCount + 1, 0);

        int y = ListStartY;
        for (int i = startI; i >= endI; --i)
        {
            const auto& e = log[i];

            // Day / Time 태그
            std::string tag = "Day " + std::to_string(e.day)
                            + "  " + std::to_string(e.time) + "시";
            uiManager.Add(std::make_unique<UILabel>(
                LogLayout::ListX, y, LogLayout::Z,
                LogLayout::TimeW, LogLayout::RowH, tag,
                8, UILabel::TextAlign::Left, UILabel::VAlign::Middle));

            // 텍스트
            uiManager.Add(std::make_unique<UILabel>(
                LogLayout::TextX, y, LogLayout::Z,
                LogLayout::TextW, LogLayout::RowH, e.text,
                7, UILabel::TextAlign::Left, UILabel::VAlign::Middle));

            y += LogLayout::EntryH;
        }

        // ── 스크롤 힌트 (키보드 방향키 안내) ────────────────────────────────
        bool canUp = (startI < total - 1);   // 더 새로운 항목 있음
        bool canDn = (endI > 0);             // 더 오래된 항목 있음

        if (canUp)
        {
            uiManager.Add(std::make_unique<UILabel>(
                LogLayout::ScrollX, LogLayout::ScrollUpY, LogLayout::Z,
                8, LogLayout::RowH, "^",
                8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
        }
        if (canDn)
        {
            uiManager.Add(std::make_unique<UILabel>(
                LogLayout::ScrollX, LogLayout::ScrollDnY, LogLayout::Z,
                8, LogLayout::RowH, "v",
                8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
        }
    }

    // ── 닫기 버튼 ─────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UIButton>(
        LogLayout::CloseX, LogLayout::CloseY,
        LogLayout::CloseW, LogLayout::RowH, LogLayout::Z,
        L("ui.close"),
        [this]()
        {
            context.sound.PlaySE("Assets/audio/ui_button_click.wav");
            context.PopState();
        }));
}

void LogState::HandleInput(InputManager& input)
{
    while (input.HasAction())
    {
        auto action = input.PopAction();
        switch (action)
        {
        case InputAction::Cancel:
            context.PopState();
            return;

        case InputAction::MoveUp:
        case InputAction::ScrollUp:
        {
            // 위 = 더 최신 항목으로
            if (scrollOffset > 0)
            {
                --scrollOffset;
                Rebuild();
            }
            break;
        }

        case InputAction::MoveDown:
        case InputAction::ScrollDown:
        {
            // 아래 = 더 오래된 항목으로
            int total = static_cast<int>(context.log.size());
            int maxOffset = std::max(0, total - VisibleCount);
            if (scrollOffset < maxOffset)
            {
                ++scrollOffset;
                Rebuild();
            }
            break;
        }

        default:
            break;
        }
    }
}

void LogState::Update() {}

void LogState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
