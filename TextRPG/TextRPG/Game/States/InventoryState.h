#pragma once
#include "Core/State.h"
#include "Ui/UIManager.h"
#include "Ui/UIButton.h"
#include "Ui/UILabel.h"
#include <string>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
//  InventoryState  —  소지품 화면
//
//  x=0 ───── x=59 ────────────────── x=151 ──── x=191
//  │  플레이어 스탯  │        아이템 목록        │  아이템 상세  │
//  │ 체력 / 돈 등    │  [아이템명]  ×수량         │  설명 + 사용  │
//
//  StoryState 의 "소지품" 버튼 → context.PushState(InventoryState)
//  ESC / 닫기 버튼             → context.PopState()
// ─────────────────────────────────────────────────────────────────────────────
class InventoryState : public State
{
public:
	explicit InventoryState(Context& context);

	void Enter()                          override;
	void Exit()                           override {}
	void Resume()                         override;

	void HandleInput(InputManager& input)  override;
	void Update()                         override;
	void Render(ConsoleDisplay& display)   override;

private:
	// ── 패널 빌더 ────────────────────────────────────────────────────────────
	void BuildDividers();
	void BuildLeftPanel();    // 플레이어 스탯
	void BuildCenterPanel();  // 아이템 목록
	void BuildRightPanel();   // 아이템 상세 + 사용 버튼

	// ── 상세 패널만 교체 ──────────────────────────────────────────────────────
	void RebuildDetailPanel();

	// ── 선택 아이템 ────────────────────────────────────────────────────────
	std::string selectedItemId;   // 비어 있으면 "선택 없음"

	// ── 애니메이션 ─────────────────────────────────────────────────────────
	float elapsedTime = 0.f;
};
