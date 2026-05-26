#define NOMINMAX
#include "CharacterState.h"

#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UILabel.h"
#include "Ui/UIButton.h"

#include <string>
#include <algorithm>

namespace ChLayout
{
    constexpr int Z        = 10;
    constexpr int RowH     = 3;

    constexpr int TitleY   = 1;
    constexpr int DivX     = 62;

    // 좌측: 인물 목록
    constexpr int ListX      = 2;
    constexpr int ListW      = 58;
    constexpr int ListStartY = 7;
    constexpr int MaxItems   = 13;

    // 우측: 인물 상세
    constexpr int RightX       = 65;
    constexpr int RightW       = 124;
    constexpr int NameY        = 7;
    constexpr int RoleY        = 11;
    constexpr int AffiliationY = 15;
    constexpr int RelationY    = 19;
    constexpr int DetailDivY   = 23;
    constexpr int DescY        = 25;
    constexpr int DescH        = 22;

    constexpr int CloseX = 83;
    constexpr int CloseW = 25;
    constexpr int CloseY = 50;
}

CharacterState::CharacterState(Context& context)
    : State(context)
{
}

void CharacterState::Enter()
{
    selectedIndex = context.characters.empty() ? -1 : 0;
    Rebuild();
}

void CharacterState::Rebuild()
{
    uiManager.Clear();

    // ── 전체 제목 ─────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        0, ChLayout::TitleY, ChLayout::Z,
        192, ChLayout::RowH, L("ui.characters"),
        14, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 세로 구분선 ───────────────────────────────────────────────────────────
    for (int row = 5; row < 50; ++row)
    {
        uiManager.Add(std::make_unique<UILabel>(
            ChLayout::DivX, row, ChLayout::Z,
            2, 1, "|",
            8, UILabel::TextAlign::Left, UILabel::VAlign::Top));
    }

    // ── 좌측: 인물 목록 제목 ──────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        ChLayout::ListX, 5, ChLayout::Z,
        ChLayout::ListW, ChLayout::RowH, L("ui.character_list"),
        8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    if (context.characters.empty())
    {
        uiManager.Add(std::make_unique<UILabel>(
            ChLayout::ListX, ChLayout::ListStartY, ChLayout::Z,
            ChLayout::ListW, ChLayout::RowH, L("ui.character_empty"),
            8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
    }
    else
    {
        int y = ChLayout::ListStartY;
        int count = std::min(static_cast<int>(context.characters.size()), ChLayout::MaxItems);

        for (int i = 0; i < count; ++i)
        {
            const auto& ch = context.characters[i];
            int idx = i;

            auto btn = std::make_unique<UIButton>(
                ChLayout::ListX, y,
                ChLayout::ListW, ChLayout::RowH, ChLayout::Z,
                ch.name,
                [this, idx]()
                {
                    context.sound.PlaySE("Assets/audio/ui_button_click.wav");
                    selectedIndex = idx;
                    Rebuild();
                });

            uiManager.Add(std::move(btn));
            y += ChLayout::RowH;
        }
    }

    // ── 우측: 인물 상세 헤더 ──────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        ChLayout::RightX, 5, ChLayout::Z,
        ChLayout::RightW, ChLayout::RowH, L("ui.character_detail"),
        8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(context.characters.size()))
    {
        const auto& ch = context.characters[selectedIndex];

        // 이름
        uiManager.Add(std::make_unique<UILabel>(
            ChLayout::RightX, ChLayout::NameY, ChLayout::Z,
            ChLayout::RightW, ChLayout::RowH, ch.name,
            14, UILabel::TextAlign::Left, UILabel::VAlign::Middle));

        // 직위/역할
        if (!ch.role.empty())
        {
            std::string roleStr = L("ui.character_role") + "  " + ch.role;
            uiManager.Add(std::make_unique<UILabel>(
                ChLayout::RightX, ChLayout::RoleY, ChLayout::Z,
                ChLayout::RightW, ChLayout::RowH, roleStr,
                8, UILabel::TextAlign::Left, UILabel::VAlign::Middle));
        }

        // 소속
        if (!ch.affiliation.empty())
        {
            std::string affStr = L("ui.character_affiliation") + "  " + ch.affiliation;
            uiManager.Add(std::make_unique<UILabel>(
                ChLayout::RightX, ChLayout::AffiliationY, ChLayout::Z,
                ChLayout::RightW, ChLayout::RowH, affStr,
                8, UILabel::TextAlign::Left, UILabel::VAlign::Middle));
        }

        // 플레이어와의 관계
        if (!ch.relationship.empty())
        {
            std::string relStr = L("ui.character_relationship") + "  " + ch.relationship;
            uiManager.Add(std::make_unique<UILabel>(
                ChLayout::RightX, ChLayout::RelationY, ChLayout::Z,
                ChLayout::RightW, ChLayout::RowH, relStr,
                11, UILabel::TextAlign::Left, UILabel::VAlign::Middle));
        }

        // 가로 구분선
        uiManager.Add(std::make_unique<UILabel>(
            ChLayout::RightX, ChLayout::DetailDivY, ChLayout::Z,
            ChLayout::RightW, 1, std::string(62, '-'),
            8, UILabel::TextAlign::Left, UILabel::VAlign::Top));

        // 설명
        if (!ch.description.empty())
        {
            uiManager.Add(std::make_unique<UILabel>(
                ChLayout::RightX, ChLayout::DescY, ChLayout::Z,
                ChLayout::RightW, ChLayout::DescH, ch.description,
                7, UILabel::TextAlign::Left, UILabel::VAlign::Top));
        }
    }
    else if (!context.characters.empty())
    {
        uiManager.Add(std::make_unique<UILabel>(
            ChLayout::RightX, 25, ChLayout::Z,
            ChLayout::RightW, ChLayout::RowH, L("ui.character_select_hint"),
            8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
    }

    // ── 닫기 버튼 ─────────────────────────────────────────────────────────────
    uiManager.Add(std::make_unique<UIButton>(
        ChLayout::CloseX, ChLayout::CloseY,
        ChLayout::CloseW, ChLayout::RowH, ChLayout::Z,
        L("ui.close"),
        [this]()
        {
            context.sound.PlaySE("Assets/audio/ui_button_click.wav");
            context.PopState();
        }));
}

void CharacterState::HandleInput(InputManager& input)
{
    while (input.HasAction())
    {
        auto action = input.PopAction();
        if (action == InputAction::Cancel)
            context.PopState();
    }
}

void CharacterState::Update() {}

void CharacterState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
