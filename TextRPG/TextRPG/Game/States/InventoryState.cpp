#include "InventoryState.h"

#include "Core/Context.h"
#include "Core/InputManager.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Game/Item/Item.h"
#include "Game/Item/ItemRegistry.h"
#include "Game/Effect/EffectInterpreter.h"
#include "Ui/UILabel.h"
#include "Ui/UIButton.h"

#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

// ─────────────────────────────────────────────────────────────────────────────
//  레이아웃 상수
//
//  StoryState 와 동일한 분할선(x=59, x=151) 을 사용합니다.
//
//  x=0 ─── x=59 ─── x=61 ─────────────── x=151 ─── x=153 ──── x=191
//  │   좌측 스탯   │       중앙 아이템 목록       │      우측 상세      │
// ─────────────────────────────────────────────────────────────────────────────
namespace InvLayout
{
	constexpr int Z = 10;
	constexpr int RowH = 3;

	// ── 좌측 (플레이어 스탯) ─────────────────────────────────────────────────
	constexpr int LeftX = 2;
	constexpr int LeftW = 55;

	// ── 중앙 (아이템 목록) ───────────────────────────────────────────────────
	constexpr int CenterX = 62;
	constexpr int CenterW = 87;
	constexpr int ListStartY = 8;   // 첫 번째 아이템 버튼 Y
	constexpr int MaxListItems = 12;  // 화면에 보여줄 최대 줄 수

	// ── 우측 (아이템 상세) ───────────────────────────────────────────────────
	constexpr int RightX = 154;
	constexpr int RightW = 36;

	// ── 공통 ─────────────────────────────────────────────────────────────────
	constexpr int TitleY = 1;   // 패널 제목 Y
	constexpr int DividerY = 4;   // 제목 아래 구분선 Y
	constexpr int ContentY = 6;   // 본문 시작 Y (스탯, 상세)
}

// ─────────────────────────────────────────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────────────────────────────────────────
InventoryState::InventoryState(Context& context)
	: State(context)
{
}

// ─────────────────────────────────────────────────────────────────────────────
//  Enter — 첫 진입 시 전체 UI 구성
// ─────────────────────────────────────────────────────────────────────────────
void InventoryState::Enter()
{
	selectedItemId.clear();

	uiManager.Clear();
	BuildDividers();
	BuildLeftPanel();
	BuildCenterPanel();
	BuildRightPanel();   // 빈 상태 (아무것도 선택 안 됨)
}

// ─────────────────────────────────────────────────────────────────────────────
//  Resume — 단준 재빌드
// ─────────────────────────────────────────────────────────────────────────────
void InventoryState::Resume()
{
	Enter();
}

// ─────────────────────────────────────────────────────────────────────────────
//  BuildDividers 수직 분할선
// ─────────────────────────────────────────────────────────────────────────────
void InventoryState::BuildDividers()
{
	for ( int row = 0; row < 54; ++row )
	{
		// 좌/중앙 경계
		uiManager.Add(std::make_unique<UILabel>(
			59 , row , InvLayout::Z , 2 , 1 ,
			"|" , 8 ,
			UILabel::TextAlign::Left ,
			UILabel::VAlign::Top));

		// 중앙/우측 경계
		uiManager.Add(std::make_unique<UILabel>(
			151 , row , InvLayout::Z , 2 , 1 ,
			"|" , 8 ,
			UILabel::TextAlign::Left ,
			UILabel::VAlign::Top));
	}
}

// ─────────────────────────────────────────────────────────────────────────────
//  BuildLeftPanel — 플레이어 스탯 표시
// ─────────────────────────────────────────────────────────────────────────────
void InventoryState::BuildLeftPanel()
{
	const auto& p = context.player;

	// ── 패널 제목 ──────────────────────────────────────────────────────────
	uiManager.Add(std::make_unique<UILabel>(
		InvLayout::LeftX , InvLayout::TitleY , InvLayout::Z ,
		InvLayout::LeftW , InvLayout::RowH ,
		L("ui.player_status") , 15 ,
		UILabel::TextAlign::Center , UILabel::VAlign::Middle));

	uiManager.Add(std::make_unique<UILabel>(
		InvLayout::LeftX , InvLayout::DividerY , InvLayout::Z ,
		InvLayout::LeftW , 1 ,
		std::string(InvLayout::LeftW , '-') , 8 ,
		UILabel::TextAlign::Left , UILabel::VAlign::Top));

	// ── 스탯 항목 ──────────────────────────────────────────────────────────
	struct StatRow { std::string labelKey; int value; };
	const std::vector<StatRow> rows =
	{
		{ "stat.vitality",   p.vitality   },
		{ "stat.appearance", p.appearance },
		{ "stat.reputation", p.reputation },
		{ "stat.karma",      p.karma      },
		{ "stat.wealth",     p.wealth     },
		{ "stat.day",        p.day        },
		{ "stat.time",       p.time       },
	};

	int y = InvLayout::ContentY;
	for ( const auto& row : rows )
	{
		// 레이블
		uiManager.Add(std::make_unique<UILabel>(
			InvLayout::LeftX , y , InvLayout::Z ,
			30 , InvLayout::RowH ,
			L(row.labelKey) , 7 ,
			UILabel::TextAlign::Left , UILabel::VAlign::Middle));

		// 값
		uiManager.Add(std::make_unique<UILabel>(
			InvLayout::LeftX + 30 , y , InvLayout::Z ,
			InvLayout::LeftW - 30 , InvLayout::RowH ,
			std::to_string(row.value) , 11 ,  // 밝은 시안
			UILabel::TextAlign::Left , UILabel::VAlign::Middle));

		y += InvLayout::RowH;
	}

	// ── 닫기 버튼 (좌측 패널 하단) ────────────────────────────────────────
	auto closeBtn = std::make_unique<UIButton>(
		InvLayout::LeftX , 48 , InvLayout::LeftW , InvLayout::RowH ,
		InvLayout::Z ,
		L("ui.close") ,
		[ this ] ()
		{
			context.sound.PlaySE("Assets/audio/ui_button_click.wav");
			context.PopState();
		});

	uiManager.Add(std::move(closeBtn));
}

// ─────────────────────────────────────────────────────────────────────────────
//  BuildCenterPanel — 소지 아이템 목록
// ─────────────────────────────────────────────────────────────────────────────
void InventoryState::BuildCenterPanel()
{
	const auto& inv = context.player.inventory;

	// ── 패널 제목 ──────────────────────────────────────────────────────────
	std::string titleText = L("ui.inventory");
	titleText += "  (" + std::to_string(inv.size()) + L("ui.item_count_suffix") + ")";

	uiManager.Add(std::make_unique<UILabel>(
		InvLayout::CenterX , InvLayout::TitleY , InvLayout::Z ,
		InvLayout::CenterW , InvLayout::RowH ,
		titleText , 15 ,
		UILabel::TextAlign::Center , UILabel::VAlign::Middle));

	uiManager.Add(std::make_unique<UILabel>(
		InvLayout::CenterX , InvLayout::DividerY , InvLayout::Z ,
		InvLayout::CenterW , 1 ,
		std::string(InvLayout::CenterW , '-') , 8 ,
		UILabel::TextAlign::Left , UILabel::VAlign::Top));

	// ── 비어있을 때 ────────────────────────────────────────────────────────
	if ( inv.empty() )
	{
		uiManager.Add(std::make_unique<UILabel>(
			InvLayout::CenterX , InvLayout::ListStartY , InvLayout::Z ,
			InvLayout::CenterW , InvLayout::RowH ,
			L("ui.inventory_empty") , 8 ,
			UILabel::TextAlign::Center , UILabel::VAlign::Middle));
		return;
	}

	// ── 아이템 버튼 목록 ───────────────────────────────────────────────────
	// inventory 를 정렬된 순서로 표시
	std::vector<std::pair<std::string , int>> sortedItems(inv.begin() , inv.end());
	std::sort(sortedItems.begin() , sortedItems.end() ,
		[] (const auto& a , const auto& b) { return a.first < b.first; });

	int y = InvLayout::ListStartY;
	for ( const auto& [itemId , qty] : sortedItems )
	{
		const Item& item = ItemRegistry::Get(itemId);
		std::string btnText = L(item.nameKey) + "  x" + std::to_string(qty);

		bool isSelected = ( itemId == selectedItemId );

		// 선택된 아이템은 밝은 노랑(14)으로 강조
		auto btn = std::make_unique<UIButton>(
			InvLayout::CenterX , y ,
			InvLayout::CenterW , InvLayout::RowH ,
			InvLayout::Z ,
			btnText ,
			[ this , itemId ] ()
			{
				context.sound.PlaySE("Assets/audio/ui_button_click.wav");
				selectedItemId = itemId;
				RebuildDetailPanel();
				needsRebuild = true;
			});

		btn->borderless = isSelected ? false : true;
		btn->StartFadeIn(10.f);
		uiManager.Add(std::move(btn));

		y += InvLayout::RowH + 1;
		if ( y >= InvLayout::ListStartY + InvLayout::MaxListItems * ( InvLayout::RowH + 1 ) )
			break;  // 화면 밖이면 중단 (스크롤은 추후 구현)
	}
}

// ─────────────────────────────────────────────────────────────────────────────
//  BuildRightPanel — 선택된 아이템 상세 + 사용 버튼
// ─────────────────────────────────────────────────────────────────────────────
void InventoryState::BuildRightPanel()
{
	// ── 패널 제목 ──────────────────────────────────────────────────────────
	uiManager.Add(std::make_unique<UILabel>(
		InvLayout::RightX , InvLayout::TitleY , InvLayout::Z ,
		InvLayout::RightW , InvLayout::RowH ,
		L("ui.item_detail") , 15 ,
		UILabel::TextAlign::Center , UILabel::VAlign::Middle));

	uiManager.Add(std::make_unique<UILabel>(
		InvLayout::RightX , InvLayout::DividerY , InvLayout::Z ,
		InvLayout::RightW , 1 ,
		std::string(InvLayout::RightW , '-') , 8 ,
		UILabel::TextAlign::Left , UILabel::VAlign::Top));

	// ── 아이템 미선택 ──────────────────────────────────────────────────────
	if ( selectedItemId.empty() )
	{
		uiManager.Add(std::make_unique<UILabel>(
			InvLayout::RightX , InvLayout::ContentY , InvLayout::Z ,
			InvLayout::RightW , InvLayout::RowH ,
			L("ui.select_item_hint") , 8 ,
			UILabel::TextAlign::Center , UILabel::VAlign::Middle));
		return;
	}

	RebuildDetailPanel();
}

// ─────────────────────────────────────────────────────────────────────────────
//  RebuildDetailPanel — 우측 상세 영역만 갱신
//  (BuildRightPanel 의 헤더/구분선 이후 내용 부분)
// ─────────────────────────────────────────────────────────────────────────────
void InventoryState::RebuildDetailPanel()
{
	if ( selectedItemId.empty() ) return;

	const Item& item = ItemRegistry::Get(selectedItemId);
	auto it = context.player.inventory.find(selectedItemId);
	int  qty = ( it != context.player.inventory.end() ) ? it->second : 0;

	int y = InvLayout::ContentY;

	// 아이템 이름
	uiManager.Add(std::make_unique<UILabel>(
		InvLayout::RightX , y , InvLayout::Z ,
		InvLayout::RightW , InvLayout::RowH ,
		L(item.nameKey) , 14 ,   // 밝은 노랑
		UILabel::TextAlign::Left , UILabel::VAlign::Middle));
	y += InvLayout::RowH;

	// 소지 수량
	uiManager.Add(std::make_unique<UILabel>(
		InvLayout::RightX , y , InvLayout::Z ,
		InvLayout::RightW , InvLayout::RowH ,
		L("ui.quantity") + ": " + std::to_string(qty) , 7 ,
		UILabel::TextAlign::Left , UILabel::VAlign::Middle));
	y += InvLayout::RowH + 1;

	// 구분선
	uiManager.Add(std::make_unique<UILabel>(
		InvLayout::RightX , y , InvLayout::Z ,
		InvLayout::RightW , 1 ,
		std::string(InvLayout::RightW , '-') , 8 ,
		UILabel::TextAlign::Left , UILabel::VAlign::Top));
	y += 2;

	// 아이템 설명 (자동 줄바꿈은 UILabel 내부가 처리)
	uiManager.Add(std::make_unique<UILabel>(
		InvLayout::RightX , y , InvLayout::Z ,
		InvLayout::RightW , 12 ,
		L(item.descKey) , 7 ,
		UILabel::TextAlign::Left , UILabel::VAlign::Top));
	y += 13;

	// ── 사용하기 버튼 ────────────────────────────────────────────────────
	if ( item.usable && qty > 0 )
	{
		auto useBtn = std::make_unique<UIButton>(
			InvLayout::RightX , y ,
			InvLayout::RightW , InvLayout::RowH ,
			InvLayout::Z ,
			L("ui.use_item") ,
			[ this ] ()
			{
				context.sound.PlaySE("Assets/audio/ui_button_click.wav");

				const Item& it = ItemRegistry::Get(selectedItemId);

				// 1. 이펙트 적용
				for ( const auto& effect : it.onUseEffects )
					EffectInterpreter::Apply(effect , context);

				// 2. 데이터 처리
				auto& inv = context.player.inventory;
				inv[ selectedItemId ]--;
				if ( inv[ selectedItemId ] <= 0 )
				{
					inv.erase(selectedItemId);
					selectedItemId.clear();
				}

				// 3. [중요] 즉시 재빌드하지 않고 플래그만 설정
				this->needsRebuild = true;
			});

		useBtn->StartFadeIn();
		uiManager.Add(std::move(useBtn));
		y += InvLayout::RowH + 1;
	}
	else if ( !item.usable )
	{
		uiManager.Add(std::make_unique<UILabel>(
			InvLayout::RightX , y , InvLayout::Z ,
			InvLayout::RightW , InvLayout::RowH ,
			L("ui.key_item_hint") , 8 ,
			UILabel::TextAlign::Left , UILabel::VAlign::Middle));
	}
}

// ─────────────────────────────────────────────────────────────────────────────
//  HandleInput
// ─────────────────────────────────────────────────────────────────────────────
void InventoryState::HandleInput(InputManager& input)
{
	while ( input.HasAction() )
	{
		auto action = input.PopAction();

		if ( action == InputAction::Quit )
		{
			context.sound.PlaySE("Assets/audio/ui_button_click.wav");
			context.PopState();
			return;
		}
	}
}

// ─────────────────────────────────────────────────────────────────────────────
//  Update
// ─────────────────────────────────────────────────────────────────────────────
void InventoryState::Update()
{
	auto now = Clock::now();
	if ( lastTime != Clock::time_point{} )
	{
		float dt = std::chrono::duration<float>(now - lastTime).count();
		if ( dt > 0.f && dt < 1.f )
			uiManager.Update(1.f / dt);
	}
	lastTime = now;

	if ( needsRebuild ) {
		uiManager.Clear();
		BuildDividers();
		BuildLeftPanel();
		BuildCenterPanel();
		BuildRightPanel();
		needsRebuild = false;
	}
}

// ─────────────────────────────────────────────────────────────────────────────
//  Render
// ─────────────────────────────────────────────────────────────────────────────
void InventoryState::Render(ConsoleDisplay& display)
{
	uiManager.Render(display);
}