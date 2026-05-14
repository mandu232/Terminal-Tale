#include "StoryState.h"
#include "SettingState.h"
#include "InventoryState.h"

#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UIButton.h"
#include "Ui/UILabel.h"
#include "Ui/UIImage.h"
#include "Ui/UITypewriter.h"

#include <chrono>
#include "Game/Story/StoryLoader.h"
#include "Systems/ConditionChecker.h"
#include "Game/Effect/EffectInterpreter.h"

#include <algorithm>
#include <string>

// ─────────────────────────────────────────────
//  레이아웃 상수
//
//  전체 화면을 3개 패널로 분할합니다.
//
//  x=0 ─────── x=60 ───────────── x=152 ──── x=190
//  │  좌측 이미지  │     중앙 텍스트+선택지    │ 우측 퀵메뉴 │
//
//  UILabel  생성자: (x, y, z, width, height, text, color, align, valign)
//  UIButton 생성자: (x, y, w, h, z, text, callback)
//  UIImage  생성자: (x, y, z, path, color=7)          ← width/height 없음
// ─────────────────────────────────────────────
namespace Layout
{
	constexpr int Z = 10;
	constexpr int RowH = 3;

	// ── 좌측 패널 (배경 이미지) ──────────────────
	constexpr int LeftX = 0;
	constexpr int LeftW = 58;
	constexpr int LeftH = 58;   // 화면 높이에 맞게 조정
	constexpr int LeftY = 0;

	// ── 중앙 패널 (텍스트 + 선택지) ─────────────
	constexpr int CenterX = 61;
	constexpr int CenterW = 88;
	constexpr int TextStartY = 4;

	// 텍스트와 선택지 사이 구분선
	constexpr int DividerY = 38;     // 선택지 위 구분선 Y
	constexpr int ChoiceStartY = 42;    // 선택지 첫 줄 Y

	// ── 우측 패널 (퀵 메뉴) ─────────────────────
	constexpr int RightX = 156;
	constexpr int RightW = 32;
	constexpr int RightBtnW = 12;

	// 퀵메뉴 항목 Y 좌표
	constexpr int QuickMenuStartY = 4;

	// 우측 2열 레이아웃
	//  열 A (소지품 / 대기 / 수면):  RightX
	//  열 B (설정 / 빠른저장 / ...):  RightX + 18
	constexpr int ColA = RightX;
	constexpr int ColB = RightX + 18;
	constexpr int ColW = 16;

	constexpr int Row0 = QuickMenuStartY;        // 소지품 | 설정
	constexpr int Row1 = Row0 + RowH + 1;        // 대기   | 빠른 저장
	constexpr int Row2 = Row1 + RowH + 1;        // 수면   | 빠른 불러오기
	constexpr int Row3 = Row2 + RowH + 1;        //        | 기록
	constexpr int Row4 = Row3 + RowH + 1;        //        | 저널
}

// ─────────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────────
StoryState::StoryState(Context& context , const std::string& startNodeId)
	: State(context)
	, startNodeId(startNodeId)
{
}

// ─────────────────────────────────────────────
//  Enter
// ─────────────────────────────────────────────
void StoryState::Enter()
{
	currentNode = StoryLoader::Load(NodePath(startNodeId));

	BuildLeftPanel();
	BuildRightPanel();
	RebuildCenter();
	BuildDividers();
}

// ─────────────────────────────────────────────
//  NodePath
// ─────────────────────────────────────────────
std::string StoryState::NodePath(const std::string& nodeId)
{
	return "Data/Story/" + nodeId + ".json";
}

// ─────────────────────────────────────────────
//  BuildLeftPanel — 배경 이미지 (Enter 1회)
// ─────────────────────────────────────────────
void StoryState::BuildLeftPanel()
{
	uiManager.Add(
		std::make_unique<UIImage>(
			Layout::LeftX , Layout::LeftY , Layout::Z ,
			currentNode.bgImage)          // UIImage(x, y, z, path, color=7)
	);
}

// ─────────────────────────────────────────────
//  BuildRightPanel — 퀵 메뉴 (Enter 1회)
//
//  열 A   │ 열 B
//  ──────┼──────────────
//  소지품 │ 설정
//  대기   │ 빠른 저장
//  수면   │ 빠른 불러오기
//         │ 기록
//         │ 저널
// ─────────────────────────────────────────────
void StoryState::BuildRightPanel()
{
	auto addQuickBtn = [ & ] (int x , int y , std::string label , std::function<void()> cb)
		{
			auto btn = std::make_unique<UIButton>(
				x , y , Layout::ColW , Layout::RowH , Layout::Z , label , cb);
			btn->borderless = true;
			uiManager.Add(std::move(btn));
		};
	//소지품(인벤토리)
	addQuickBtn(Layout::ColA , Layout::Row0 , L("ui.inventory") , [ this ] () {
		context.sound.PlaySE("Assets/audio/ui_button_click.wav");
		context.PushState(std::make_unique< InventoryState>(context));
		});

	//대기
	addQuickBtn(Layout::ColA , Layout::Row1 , L("ui.wait") , [ this ] () {
		context.sound.PlaySE("Assets/audio/ui_button_click.wav");
		if ( onWait ) onWait(); });

	//수면
	addQuickBtn(Layout::ColA , Layout::Row2 , L("ui.sleep") , [ this ] () {
		context.sound.PlaySE("Assets/audio/ui_button_click.wav");
		if ( onSleep ) onSleep(); });

	//설정
	addQuickBtn(Layout::ColB , Layout::Row0 , L("ui.setting") , [ this ] () {
		context.sound.PlaySE("Assets/audio/ui_button_click.wav");
		context.PushState(std::make_unique<SettingState>(context));
		});

	//빠른 저장
	addQuickBtn(Layout::ColB , Layout::Row1 , L("ui.quickSave") , [ this ] () {
		context.sound.PlaySE("Assets/audio/ui_button_click.wav");
		});

	//빠른 불러오기
	addQuickBtn(Layout::ColB , Layout::Row2 , L("ui.quickLoad") , [ this ] () {
		context.sound.PlaySE("Assets/audio/ui_button_click.wav");
		});

	//로그
	addQuickBtn(Layout::ColB , Layout::Row3 , L("ui.log") , [ this ] () {
		context.sound.PlaySE("Assets/audio/ui_button_click.wav");
		if ( onLog ) onLog();
		});

	//저널
	addQuickBtn(Layout::ColB , Layout::Row4 , L("ui.journal") , [ this ] () {
		context.sound.PlaySE("Assets/audio/ui_button_click.wav");
		if ( onJournal ) onJournal();
		});
}

// ─────────────────────────────────────────────
//  NavigateTo — 노드 전환 (중앙 패널만 재구성)
// ─────────────────────────────────────────────
void StoryState::NavigateTo(const std::string& nodeId)
{
	currentNode = StoryLoader::Load(NodePath(nodeId));

	for ( const auto& effect : currentNode.effects )
		EffectInterpreter::Apply(effect , context);

	// 배경 이미지가 바뀌면 좌측 패널도 갱신
	// (UIImage 를 교체하려면 tag 기반 조회가 필요합니다.
	//  현재는 단순하게 전체 재구성합니다.)
	uiManager.Clear();
	BuildLeftPanel();
	BuildRightPanel();
	RebuildCenter();
	BuildDividers();
}

// ─────────────────────────────────────────────
//  RebuildCenter — 중앙 텍스트 + 선택지
// ─────────────────────────────────────────────
void StoryState::RebuildCenter()
{
	// ── 상태 초기화 ──────────────────────────────
	pendingTypewriters = 0;
	choiceButtons.clear();

	// ── 본문 텍스트 (UITypewriter) ───────────────
	int y = Layout::TextStartY;

	for ( const auto& line : currentNode.texts )
	{
		auto tw = std::make_unique<UITypewriter>(
			Layout::CenterX , y , Layout::Z ,
			Layout::CenterW , Layout::RowH ,
			L(line) , 7 ,
			UITypewriter::TextAlign::Left ,
			UITypewriter::VAlign::Middle ,
			context.settingManager.settings.textSpeed);                         // speed 1~5

		++pendingTypewriters;

		tw->onComplete = [ this ] ()
			{
				if ( --pendingTypewriters == 0 )
					SpawnChoices();
			};

		uiManager.Add(std::move(tw));
		y += Layout::RowH;
	}

	// ── 구분선 ───────────────────────────────────
	const std::string divider(Layout::CenterW , '-');
	uiManager.Add(
		std::make_unique<UILabel>(
			Layout::CenterX , Layout::DividerY , Layout::Z ,
			Layout::CenterW , Layout::RowH ,
			divider , 8 ,
			UILabel::TextAlign::Left ,
			UILabel::VAlign::Middle)
	);
}

// ─────────────────────────────────────────────
//  BuildDividers 
// ─────────────────────────────────────────────
void StoryState::BuildDividers()
{
	// 좌/중앙 구분선 (x=59)
	for ( int row = 0; row < 54; ++row )
	{
		uiManager.Add(std::make_unique<UILabel>(
			59 , row , Layout::Z , 2 , 1 ,
			"|" , 8 ,  // 어두운 회색
			UILabel::TextAlign::Left ,
			UILabel::VAlign::Top));
	}

	// 중앙/우측 구분선 (x=151)
	for ( int row = 0; row < 54; ++row )
	{
		uiManager.Add(std::make_unique<UILabel>(
			151 , row , Layout::Z , 2 , 1 ,
			"|" , 8 ,
			UILabel::TextAlign::Left ,
			UILabel::VAlign::Top));
	}
}


// ─────────────────────────────────────────────
//  HandleInput / Update / Render
// ─────────────────────────────────────────────
void StoryState::HandleInput(InputManager& input)
{
	while ( input.HasAction() )
	{
		auto action = input.PopAction();
		// ESC → context.PopState();
	}
}

void StoryState::Update()
{
	using Clock = std::chrono::steady_clock;
	auto now = Clock::now();

	float elapsed = 0.f;

	if ( lastFrameTime != Clock::time_point{} )
	{
		elapsed = std::chrono::duration<float>(now - lastFrameTime).count();
		if ( elapsed > 0.f && elapsed < 1.f )          // 비정상값(일시정지 등) 방어
		{
			float fps = 1.0f / elapsed;
			uiManager.Update(fps);
		}
	}

	lastFrameTime = now;

	if ( spawningChoices )
	{
		choiceSpawnTimer += elapsed;

		if ( choiceSpawnTimer >= CHOICE_SPAWN_DELAY )
		{
			choiceSpawnTimer = 0.f;

			if ( nextChoiceIndex < pendingChoices.size() )
			{
				CreateChoiceButton(
					pendingChoices[ nextChoiceIndex ].data);

				nextChoiceIndex++;
			}
			else
			{
				spawningChoices = false;
			}
		}
	}
}

void StoryState::EnableChoices()
{
	for ( auto& [btn , canUse] : choiceButtons )
		btn->SetEnabled(canUse);
}

void StoryState::Render(ConsoleDisplay& display)
{
	uiManager.Render(display);
}

void StoryState::SpawnChoices()
{
	pendingChoices.clear();

	for ( const auto& choice : currentNode.choices )
	{
		if ( !ConditionChecker::Check(choice.require , context) )
			continue;

		pendingChoices.push_back({ choice });
	}

	nextChoiceIndex = 0;
	choiceSpawnTimer = 0.f;
	spawningChoices = true;
	nextChoiceY = Layout::ChoiceStartY;
}

void StoryState::CreateChoiceButton(const StoryChoice& choice)
{
	std::string nextId = choice.nextNode;

	auto btn = std::make_unique<UIButton>(
		Layout::CenterX ,
		nextChoiceY ,
		Layout::CenterW ,
		Layout::RowH ,
		Layout::Z ,
		L(choice.text) ,
		[ this , nextId , choice ] ()
		{
			context.sound.PlaySE("Assets/audio/ui_button_click.wav");

			for ( const auto& effect : choice.effects )
				EffectInterpreter::Apply(effect , context);

			NavigateTo(nextId);
		});

	btn->StartFadeIn();

	uiManager.Add(std::move(btn));

	nextChoiceY += Layout::RowH + 1;
}