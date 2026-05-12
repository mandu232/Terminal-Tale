#pragma once

#include "Core/State.h"
#include "Ui/UIManager.h"
#include "Ui/UIButton.h"
#include "Game/Story/StoryNode.h"

#include <string>
#include <functional>

#include <chrono>
#include <vector>

// ─────────────────────────────────────────────
//  StoryState  —  로느워든 스타일 3패널 레이아웃
//
//  ┌──────────────┬──────────────────┬─────────┐
//  │  배경 이미지  │  텍스트 + 선택지  │ 퀵 메뉴 │
//  └──────────────┴──────────────────┴─────────┘
//
//  우측 퀵메뉴 콜백은 외부에서 주입합니다.
//  ex) storyState->onInventory = [](){ ... };
// ─────────────────────────────────────────────
class StoryState : public State
{
public:
	StoryState(Context& context , const std::string& startNodeId);

	// 퀵메뉴 콜백 (미설정 시 버튼은 표시되나 아무 동작 없음)
	std::function<void()> onInventory;
	std::function<void()> onWait;
	std::function<void()> onSleep;
	std::function<void()> onLog;
	std::function<void()> onJournal;

	void Enter()                          override;
	void HandleInput(InputManager& input)  override;
	void Update()                         override;
	void Render(ConsoleDisplay& display)   override;

private:
	void NavigateTo(const std::string& nodeId);
	void RebuildCenter();       // 중앙 패널만 재구성 (노드 전환 시)
	void BuildLeftPanel();      // 좌측 이미지 패널  (Enter 1회)
	void BuildRightPanel();     // 우측 퀵메뉴 패널  (Enter 1회)
	void BuildDividers();       // 테두리 패널

	static std::string NodePath(const std::string& nodeId);

	std::string startNodeId;
	StoryNode currentNode;

	//타이핑 상태
	int pendingTypewriters = 0;
	std::vector<std::pair<UIButton* , bool>> choiceButtons;
	std::chrono::steady_clock::time_point lastFrameTime{};

	// 선택지 활성화 함수
	void EnableChoices();

	void SpawnChoices();
};