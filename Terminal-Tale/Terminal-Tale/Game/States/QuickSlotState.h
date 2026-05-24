#pragma once

#include "Core/State.h"

// ─────────────────────────────────────────────
//  QuickSlotState  —  빠른 저장 / 불러오기 슬롯 선택 화면
//
//  Mode::Save : 슬롯을 선택하면 현재 지점을 저장 후 복귀
//  Mode::Load : 슬롯을 선택하면 해당 지점을 불러와 게임 재개
// ─────────────────────────────────────────────
class QuickSlotState : public State
{
public:
	enum class Mode { Save, Load };

	QuickSlotState(Context& context, Mode mode);

	void Enter()                          override;
	void HandleInput(InputManager& input) override;
	void Update()                         override;
	void Render(ConsoleDisplay& display)  override;

private:
	Mode mode;
};
