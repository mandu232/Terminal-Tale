#define NOMINMAX
#include "AchievementState.h"

#include "Core/Context.h"
#include "Core/InputManager.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Game/Achievement/AchievementManager.h"
#include "Ui/UILabel.h"
#include "Ui/UIButton.h"

#include <string>
#include <algorithm>

AchievementState::AchievementState(Context& context)
    : State(context)
{
}

void AchievementState::Enter()
{
    scrollOffset = 0;
    Rebuild();
}

void AchievementState::Rebuild()
{
    uiManager.Clear();

    const auto& all   = context.achievements.GetAll();
    int total         = static_cast<int>(all.size());
    int unlocked      = context.achievements.UnlockedCount();

    // ── 제목 ─────────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, 1, 10,
        192, 3, L("ui.achievements"),
        14, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 달성 수 ───────────────────────────────────────────────────────────────
    std::string countText = std::to_string(unlocked) + " / " + std::to_string(total);
    uiManager.Add(std::make_unique<UILabel>(
        0, 4, 10,
        192, 1, countText,
        8, UILabel::TextAlign::Center, UILabel::VAlign::Top));

    // ── 구분선 ───────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, 6, 10,
        192, 1, std::string(192, '-'),
        8, UILabel::TextAlign::Left, UILabel::VAlign::Top));

    // ── 업적 목록 ─────────────────────────────────────────────────────────────
    scrollOffset = std::max(0, std::min(scrollOffset, std::max(0, total - VisibleCount)));

    int visibleEnd = std::min(scrollOffset + VisibleCount, total);
    int y = ListStartY;

    for ( int i = scrollOffset; i < visibleEnd; ++i )
    {
        const auto& ach = all[i];

        // 달성 여부에 따라 색상·텍스트 결정
        int    nameColor = ach.unlocked ? 14 : 8;   // 노랑(14) vs 회색(8)
        int    descColor = ach.unlocked ? 7  : 8;
        std::string mark     = ach.unlocked ? "[V] " : "[ ] ";
        std::string nameText = mark + L(ach.nameKey);
        std::string descText = ach.unlocked ? L(ach.descKey) : "???";

        // 이름 행
        uiManager.Add(std::make_unique<UILabel>(
            4, y, 10,
            184, 1, nameText,
            nameColor, UILabel::TextAlign::Left, UILabel::VAlign::Top));
        y += 1;

        // 설명 행 (2줄 높이)
        uiManager.Add(std::make_unique<UILabel>(
            8, y, 10,
            180, 2, descText,
            descColor, UILabel::TextAlign::Left, UILabel::VAlign::Top));
        y += 2;

        // 간격
        y += 1;
    }

    // ── 스크롤 힌트 ───────────────────────────────────────────────────────────
    if ( scrollOffset > 0 )
    {
        uiManager.Add(std::make_unique<UILabel>(
            184, ListStartY, 10,
            6, 1, "^",
            8, UILabel::TextAlign::Center, UILabel::VAlign::Top));
    }
    if ( scrollOffset + VisibleCount < total )
    {
        uiManager.Add(std::make_unique<UILabel>(
            184, ListEndY - 1, 10,
            6, 1, "v",
            8, UILabel::TextAlign::Center, UILabel::VAlign::Top));
    }

    // ── 닫기 버튼 ─────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UIButton>(
        83, 50, 25, 3, 10,
        L("ui.back"),
        [this]()
        {
            context.sound.PlaySE("Assets/audio/ui_button_click.wav");
            context.PopState();
        }));
}

void AchievementState::HandleInput(InputManager& input)
{
    const auto& all = context.achievements.GetAll();
    int total = static_cast<int>(all.size());

    while ( input.HasAction() )
    {
        auto action = input.PopAction();
        switch ( action )
        {
        case InputAction::Cancel:
            context.sound.PlaySE("Assets/audio/ui_button_click.wav");
            context.PopState();
            return;

        case InputAction::MoveUp:
        case InputAction::ScrollUp:
            if ( scrollOffset > 0 )
            {
                --scrollOffset;
                Rebuild();
            }
            break;

        case InputAction::MoveDown:
        case InputAction::ScrollDown:
            if ( scrollOffset + VisibleCount < total )
            {
                ++scrollOffset;
                Rebuild();
            }
            break;

        default:
            break;
        }
    }
}

void AchievementState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
