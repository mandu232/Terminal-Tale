#include "QuickSlotState.h"
#include "StoryState.h"

#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UIButton.h"
#include "Ui/UILabel.h"

#include <string>

namespace QSLayout
{
	constexpr int Z      = 10;
	constexpr int SlotW  = 50;
	constexpr int SlotH  = 4;
	constexpr int SlotX  = (192 - SlotW) / 2;
	constexpr int Slot1Y = 10;
	constexpr int SlotGap = SlotH + 2;   // 슬롯 간 간격

	constexpr int BackW  = 25;
	constexpr int BackX  = (192 - BackW) / 2;
	// BackY = Slot1Y + kQuickSlotCount * SlotGap  (동적 계산)
}

QuickSlotState::QuickSlotState(Context& context, Mode mode)
	: State(context), mode(mode)
{
}

void QuickSlotState::Enter()
{
	// ── 타이틀 ───────────────────────────────────────────────────────────────
	const std::string title = (mode == Mode::Save)
		? L("ui.quickSave")
		: L("ui.quickLoad");

	uiManager.Add(std::make_unique<UILabel>(
		0, 3, QSLayout::Z, 192, 3,
		title,
		14, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

	// ── 슬롯 목록 ─────────────────────────────────────────────────────────────
	for (int i = 1; i <= Context::kQuickSlotCount; ++i)
	{
		int y    = QSLayout::Slot1Y + (i - 1) * QSLayout::SlotGap;
		auto info = context.GetQuickSlotInfo(i);

		std::string label = "Q" + std::to_string(i) + "  —  ";

		if (info.exists)
		{
			label += "Day " + std::to_string(info.day)
			       + "  " + std::to_string(info.time) + "\xec\x8b\x9c"; // "시" UTF-8
		}
		else
		{
			label += L("ui.slot_no_data");
		}

		if (mode == Mode::Save)
		{
			// 저장 모드: 빈 슬롯 포함 모든 슬롯 클릭 가능
			int slotIdx = i;
			uiManager.Add(std::make_unique<UIButton>(
				QSLayout::SlotX, y,
				QSLayout::SlotW, QSLayout::SlotH, QSLayout::Z,
				label,
				[this, slotIdx]()
				{
					context.sound.PlaySE("Assets/audio/ui_button_click.wav");
					context.SaveQuick(slotIdx);
					context.AddLog(L("system.save.completed"));
					context.PopState();
				}));
		}
		else
		{
			// 불러오기 모드: 데이터 있는 슬롯만 클릭 가능
			if (info.exists)
			{
				int slotIdx = i;
				uiManager.Add(std::make_unique<UIButton>(
					QSLayout::SlotX, y,
					QSLayout::SlotW, QSLayout::SlotH, QSLayout::Z,
					label,
					[this, slotIdx]()
					{
						context.sound.PlaySE("Assets/audio/ui_button_click.wav");
						context.LoadQuick(slotIdx);
						context.ChangeState(
							std::make_unique<StoryState>(context, context.currentNodeId));
					}));
			}
			else
			{
				uiManager.Add(std::make_unique<UILabel>(
					QSLayout::SlotX, y, QSLayout::Z,
					QSLayout::SlotW, QSLayout::SlotH,
					label,
					8, UILabel::TextAlign::Center, UILabel::VAlign::Middle));
			}
		}
	}

	// ── 뒤로 버튼 ─────────────────────────────────────────────────────────────
	const int backY = QSLayout::Slot1Y + Context::kQuickSlotCount * QSLayout::SlotGap;
	uiManager.Add(std::make_unique<UIButton>(
		QSLayout::BackX, backY,
		QSLayout::BackW, 3, QSLayout::Z,
		L("ui.back"),
		[this]()
		{
			context.sound.PlaySE("Assets/audio/ui_button_click.wav");
			context.PopState();
		}));
}

void QuickSlotState::HandleInput(InputManager& input)
{
	while (input.HasAction())
	{
		auto action = input.PopAction();
		if (action == InputAction::Cancel)
			context.PopState();
	}
}

void QuickSlotState::Update() {}

void QuickSlotState::Render(ConsoleDisplay& display)
{
	uiManager.Render(display);
}
