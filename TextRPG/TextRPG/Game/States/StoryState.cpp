#include "StoryState.h"
#include "TitleState.h"

#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Core/UIButton.h"
#include "Core/UILabel.h"
#include "Game/Story/StoryLoader.h"

#include <string>
#include <algorithm>


// ─────────────────────────────────────────────
//  레이아웃 상수
//
//  UILabel  생성자: (x, y, z, width, height, text, color, align, valign)
//  UIButton 생성자: (x, y, w, h, z, text, callback)
//
//  화면 구성
//  ┌──────────────────────────────────────────┐
//  │  [ 노드 ID 타이틀 ]                       │  y=4
//  │  ─────────────────────────────────────── │
//  │  텍스트 줄 0                              │  y=10
//  │  텍스트 줄 1                              │  y=13
//  │  텍스트 줄 2                              │  y=16  ...
//  │                                          │
//  │  [ 선택지 0 버튼 ]                        │  y=ChoiceStartY
//  │  [ 선택지 1 버튼 ]                        │  y=ChoiceStartY+RowH
//  │  ...                                     │
//  │                    [ 계속 / 닫기 ]        │  (선택지 없을 때)
//  └──────────────────────────────────────────┘
// ─────────────────────────────────────────────
namespace Layout
{
    constexpr int Z            = 10;
    constexpr int RowH         = 3;

    // 타이틀
    constexpr int TitleX       = 10;
    constexpr int TitleY       = 4;
    constexpr int TitleW       = 160;

    // 본문 텍스트
    constexpr int TextX        = 10;
    constexpr int TextStartY   = 12;
    constexpr int TextW        = 160;

    // 선택지 버튼
    constexpr int ChoiceX      = 10;
    constexpr int ChoiceW      = 160;
    constexpr int MaxTextLines = 6;   // 텍스트가 이 줄 이하면 선택지를 고정 Y 에 배치
    constexpr int ChoiceFixedY = 32;  // 텍스트 줄 수에 관계없이 최소 이 Y 이하에 배치

    // "닫기" 버튼 (선택지 없을 때)
    constexpr int CloseW       = 16;
    constexpr int CloseX       = 152; // 우측 정렬
}

// ─────────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────────
StoryState::StoryState(Context& context, const std::string& startNodeId)
    : State(context)
    , startNodeId(startNodeId)
{
}

// ─────────────────────────────────────────────
//  Enter
// ─────────────────────────────────────────────
void StoryState::Enter()
{
    NavigateTo(startNodeId);
}

// ─────────────────────────────────────────────
//  NodePath — 노드 ID → JSON 파일 경로
// ─────────────────────────────────────────────
std::string StoryState::NodePath(const std::string& nodeId)
{
    return "Data/Story/" + nodeId + ".json";
}

// ─────────────────────────────────────────────
//  NavigateTo — 노드 로드 후 UI 재구성
// ─────────────────────────────────────────────
void StoryState::NavigateTo(const std::string& nodeId)
{
    currentNode = StoryLoader::Load(NodePath(nodeId));
    RebuildUI();
}

// ─────────────────────────────────────────────
//  RebuildUI — UI 전체 재구성
//  (노드가 바뀔 때마다 호출)
// ─────────────────────────────────────────────
void StoryState::RebuildUI()
{
    uiManager.Clear(); // 이전 노드 UI 전부 제거

    // ── 타이틀 (노드 ID 표시) ────────────────────
    uiManager.Add(
        std::make_unique<UILabel>(
            Layout::TitleX, Layout::TitleY, Layout::Z,
            Layout::TitleW, Layout::RowH,
            currentNode.id, 14,
            UILabel::TextAlign::Center,
            UILabel::VAlign::Middle)
    );

    // ── 본문 텍스트 ──────────────────────────────
    int y = Layout::TextStartY;

    for (const auto& line : currentNode.texts)
    {
        uiManager.Add(
            std::make_unique<UILabel>(
                Layout::TextX, y, Layout::Z,
                Layout::TextW, Layout::RowH,
                line, 7,
                UILabel::TextAlign::Left,
                UILabel::VAlign::Middle)
        );
        y += Layout::RowH;
    }

    // ── 선택지 시작 Y 결정 ───────────────────────
    //  텍스트가 짧아도 선택지가 화면 상단에 붙지 않도록 최솟값 보장
    int choiceY = (std::max)(y + Layout::RowH, Layout::ChoiceFixedY);

    // ── 선택지 버튼 / 닫기 버튼 ─────────────────
    if (currentNode.choices.empty())
    {
        // 선택지 없음 → 닫기(타이틀로 돌아가기)
        uiManager.Add(
            std::make_unique<UIButton>(
                Layout::CloseX, choiceY,
                Layout::CloseW, Layout::RowH, Layout::Z,
                L("ui.back"),
                [this]()
                {
					context.ChangeState(std::make_unique<TitleState>(context));
                })
        );
    }
    else
    {
        for (const auto& choice : currentNode.choices)
        {
            // 람다 캡처: nextNode 를 값으로 복사
            std::string nextId = choice.nextNode;

            uiManager.Add(
                std::make_unique<UIButton>(
                    Layout::ChoiceX, choiceY,
                    Layout::ChoiceW, Layout::RowH, Layout::Z,
                    choice.text,
                    [this, nextId]()
                    {
                        NavigateTo(nextId);
                    })
            );

            choiceY += Layout::RowH;
        }
    }
}

// ─────────────────────────────────────────────
//  HandleInput / Update / Render
// ─────────────────────────────────────────────
void StoryState::HandleInput(InputManager& input)
{
    while (input.HasAction())
    {
        auto action = input.PopAction();
        // 필요 시 키보드 단축키 처리 가능
        // ex) ESC → context.PopState();
    }
}

void StoryState::Update()
{
    // 선택지 클릭 콜백에서 즉시 갱신하므로 매 프레임 갱신 불필요
}

void StoryState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
