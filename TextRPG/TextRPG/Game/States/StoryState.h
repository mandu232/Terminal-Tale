#pragma once

#include "Core/State.h"
#include "Core/UIManager.h"
#include "Game/Story/StoryNode.h"

#include <string>
#include <vector>

// ─────────────────────────────────────────────
//  StoryState
//
//  - startNodeId 로 첫 노드를 로드
//  - 텍스트를 한 줄씩 UILabel 로 표시
//  - 선택지를 UIButton 으로 표시, 클릭 시 다음 노드로 이동
//  - 선택지가 없으면 "[ 계속 ]" 버튼 → context.PopState()
// ─────────────────────────────────────────────
class StoryState : public State
{
public:
    StoryState(Context& context, const std::string& startNodeId);

    void Enter()                        override;
    void HandleInput(InputManager& input) override;
    void Update()                       override;
    void Render(ConsoleDisplay& display) override;

private:
    // 노드 로드 후 UI 전체 재구성
    void NavigateTo(const std::string& nodeId);
    void RebuildUI();

    // 노드 JSON 경로 규칙:  Data/Story/<id>.json
    static std::string NodePath(const std::string& nodeId);

    std::string startNodeId;
    StoryNode   currentNode;
};
