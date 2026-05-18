#define NOMINMAX
#include "KeyBindState.h"
#include "Core/Context.h"
#include "Core/InputManager.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UILabel.h"
#include "Ui/UIButton.h"
#include <Windows.h>
#include <string>

// ─────────────────────────────────────────────
//  VK 코드 → 표시 문자열
// ─────────────────────────────────────────────
std::string KeyBindState::VkToDisplay(int vk)
{
	if ( vk >= 'A' && vk <= 'Z' )
		return std::string(1, static_cast<char>(vk));
	if ( vk >= '0' && vk <= '9' )
		return std::string(1, static_cast<char>(vk));
	if ( vk >= VK_F1 && vk <= VK_F24 )
		return "F" + std::to_string(vk - VK_F1 + 1);

	switch ( vk )
	{
	case VK_SPACE:  return "Space";
	case VK_TAB:    return "Tab";
	case VK_INSERT: return "Ins";
	case VK_DELETE: return "Del";
	case VK_HOME:   return "Home";
	case VK_END:    return "End";
	case VK_PRIOR:  return "PgUp";
	case VK_NEXT:   return "PgDn";
	}
	return "?";
}

// ─────────────────────────────────────────────
//  레이아웃 상수
// ─────────────────────────────────────────────
namespace KBLayout
{
	constexpr int Z        = 10;
	constexpr int RowH     = 3;
	constexpr int LabelX   = 30;
	constexpr int LabelW   = 50;
	constexpr int KeyX     = 90;
	constexpr int KeyW     = 20;
	constexpr int BtnX     = 115;
	constexpr int BtnW     = 16;
	constexpr int StartY   = 12;
	constexpr int RowGap   = 5;
	constexpr int BottomY  = 50;
	constexpr int ResetX   = 129;
	constexpr int SaveX    = 150;
	constexpr int BackX    = 171;
	constexpr int BotBtnW  = 16;
}

KeyBindState::KeyBindState(Context& context)
	: State(context) {}

// ─────────────────────────────────────────────
//  Enter
// ─────────────────────────────────────────────
void KeyBindState::Enter()
{
	rebindIndex_ = -1;

	auto& kb = context.settingManager.settings.keyBindings;

	rows_ =
	{
		{ "ui.inventory", &kb.inventoryKey  },
		{ "ui.wait",      &kb.waitKey       },
		{ "ui.sleep",     &kb.sleepKey      },
		{ "ui.log",       &kb.logKey        },
		{ "ui.journal",   &kb.journalKey    },
		{ "ui.quickSave", &kb.quickSaveKey  },
		{ "ui.quickLoad", &kb.quickLoadKey  },
	};

	// 제목
	uiManager.Add(std::make_unique<UILabel>(
		0, 4, KBLayout::Z,
		192, KBLayout::RowH, L("ui.keybind"), 14,
		UILabel::TextAlign::Center, UILabel::VAlign::Middle));

	// 열 헤더
	uiManager.Add(std::make_unique<UILabel>(
		KBLayout::LabelX, 8, KBLayout::Z,
		KBLayout::LabelW, KBLayout::RowH, L("ui.keybind.col_action"), 8,
		UILabel::TextAlign::Left, UILabel::VAlign::Middle));
	uiManager.Add(std::make_unique<UILabel>(
		KBLayout::KeyX, 8, KBLayout::Z,
		KBLayout::KeyW, KBLayout::RowH, L("ui.keybind.col_key"), 8,
		UILabel::TextAlign::Center, UILabel::VAlign::Middle));

	// 구분선
	uiManager.Add(std::make_unique<UILabel>(
		KBLayout::LabelX, 10, KBLayout::Z,
		KBLayout::BtnX + KBLayout::BtnW - KBLayout::LabelX, 1,
		std::string(KBLayout::BtnX + KBLayout::BtnW - KBLayout::LabelX, '-'), 8,
		UILabel::TextAlign::Left, UILabel::VAlign::Top));

	// 바인딩 행
	int y = KBLayout::StartY;
	for ( int i = 0; i < static_cast<int>(rows_.size()); ++i )
	{
		auto& row = rows_[i];

		uiManager.Add(std::make_unique<UILabel>(
			KBLayout::LabelX, y, KBLayout::Z,
			KBLayout::LabelW, KBLayout::RowH,
			L(row.labelKey), 7,
			UILabel::TextAlign::Left, UILabel::VAlign::Middle));

		{
			auto lbl = std::make_unique<UILabel>(
				KBLayout::KeyX, y, KBLayout::Z,
				KBLayout::KeyW, KBLayout::RowH,
				VkToDisplay(*row.value), 14,
				UILabel::TextAlign::Center, UILabel::VAlign::Middle);
			row.keyLabel = lbl.get();
			uiManager.Add(std::move(lbl));
		}
		{
			int idx = i;
			auto btn = std::make_unique<UIButton>(
				KBLayout::BtnX, y,
				KBLayout::BtnW, KBLayout::RowH,
				KBLayout::Z,
				L("ui.toggle"),
				[this, idx]()
				{
					context.sound.PlaySE("Assets/audio/ui_button_click.wav");
					if ( rebindIndex_ >= 0 && rebindIndex_ != idx )
						CancelRebind();
					rebindIndex_ = idx;
					rows_[idx].changeBtn->SetText(L("ui.keybind.waiting"));
				});
			row.changeBtn = btn.get();
			uiManager.Add(std::move(btn));
		}

		y += KBLayout::RowGap;
	}

	// 하단 버튼
	uiManager.Add(std::make_unique<UIButton>(
		KBLayout::ResetX, KBLayout::BottomY,
		KBLayout::BotBtnW, KBLayout::RowH, KBLayout::Z,
		L("ui.reset"),
		[this]()
		{
			context.sound.PlaySE("Assets/audio/ui_button_click_4.wav");
			CancelRebind();
			context.settingManager.settings.keyBindings = KeyBindings{};
			for ( auto& row : rows_ )
				if ( row.keyLabel ) row.keyLabel->SetText(VkToDisplay(*row.value));
		}));

	uiManager.Add(std::make_unique<UIButton>(
		KBLayout::SaveX, KBLayout::BottomY,
		KBLayout::BotBtnW, KBLayout::RowH, KBLayout::Z,
		L("ui.save"),
		[this]()
		{
			context.sound.PlaySE("Assets/audio/ui_button_click_4.wav");
			context.settingManager.Save("Data/settings.json");
		}));

	uiManager.Add(std::make_unique<UIButton>(
		KBLayout::BackX, KBLayout::BottomY,
		KBLayout::BotBtnW, KBLayout::RowH, KBLayout::Z,
		L("ui.back"),
		[this]()
		{
			context.sound.PlaySE("Assets/audio/ui_button_click.wav");
			context.PopState();
		}));
}

// ─────────────────────────────────────────────
//  CancelRebind
// ─────────────────────────────────────────────
void KeyBindState::CancelRebind()
{
	if ( rebindIndex_ >= 0 && rebindIndex_ < static_cast<int>(rows_.size()) )
		if ( rows_[rebindIndex_].changeBtn )
			rows_[rebindIndex_].changeBtn->SetText(L("ui.toggle"));
	rebindIndex_ = -1;
}

// ─────────────────────────────────────────────
//  HandleInput
// ─────────────────────────────────────────────
void KeyBindState::HandleInput(InputManager& input)
{
	// 리바인딩 대기 중 — 다음 raw key를 캡처
	if ( rebindIndex_ >= 0 )
	{
		int vk = input.lastRawVK;
		input.lastRawVK = 0;

		// 아직 아무 키도 안 눌림
		if ( vk == 0 )
		{
			while ( input.HasAction() ) input.PopAction();
			return;
		}

		// 수정자 키 무시
		if ( vk == VK_SHIFT   || vk == VK_LSHIFT   || vk == VK_RSHIFT   ||
			 vk == VK_CONTROL || vk == VK_LCONTROL || vk == VK_RCONTROL ||
			 vk == VK_MENU    || vk == VK_LMENU    || vk == VK_RMENU )
		{
			while ( input.HasAction() ) input.PopAction();
			return;
		}

		// 액션 큐 소진 (이 키 이벤트가 action으로 흘러나가지 않도록)
		while ( input.HasAction() ) input.PopAction();

		if ( vk == VK_ESCAPE )
		{
			CancelRebind();
			return;
		}

		// 탐색 키 (ESC/Enter/화살표/W/Q/E)는 재할당 불가
		static const int kFixed[] = {
			VK_RETURN, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
			0x57, 0x51, 0x45  // W, Q, E
		};
		for ( int fvk : kFixed )
		{
			if ( vk == fvk ) return;  // 무시 (버튼은 그대로 "대기 중...")
		}

		*rows_[rebindIndex_].value = vk;
		rows_[rebindIndex_].keyLabel->SetText(VkToDisplay(vk));
		CancelRebind();
		return;
	}

	// 일반 입력
	while ( input.HasAction() )
	{
		auto action = input.PopAction();
		if ( action == InputAction::Cancel )
		{
			context.sound.PlaySE("Assets/audio/ui_button_click.wav");
			context.PopState();
			return;
		}
	}
}

// ─────────────────────────────────────────────
//  Render
// ─────────────────────────────────────────────
void KeyBindState::Render(ConsoleDisplay& display)
{
	uiManager.Render(display);
}
