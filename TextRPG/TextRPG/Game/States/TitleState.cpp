// ──────────────────────────────────────────────────────────────────────────────
//  TitleState.cpp  (수정된 버전)
//  변경점: "새 게임" 버튼에서 StoryState 생성 시 onInventory 콜백 연결
// ──────────────────────────────────────────────────────────────────────────────
#include "TitleState.h"
#include "GameState.h"
#include "StoryState.h"
#include "SettingState.h"
#include "InventoryState.h"              // ← 추가
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Game/Events/GameStartEvent.h"
#include "Game/Events/PlaySoundEvent.h"
#include "Core/ConsoleDisplay.h"
#include "Game/Item/ItemRegistry.h"      // ← 추가
#include <iostream>
#include <Core/Localization.h>
#include "Ui/UIButton.h"
#include "Ui/UILabel.h"
#include "Ui/UIImage.h"

TitleState::TitleState(Context& context)
    : State(context)
{
}

void TitleState::Enter()
{
	// ── 아이템 데이터 로드 (타이틀 진입 시 한 번만) ──────────────────────────
	ItemRegistry::Load("Data/items.json");   // ← 추가

	playSound =
		context.eventBus.Subscribe<PlaySoundEvent>(
			[this](const PlaySoundEvent& e)
			{
				context.sound.PlaySE(e.soundName);
			});

    startSub =
        context.eventBus.Subscribe<GameStartEvent>(
            [this](const GameStartEvent&)
            {
				context.PushState(
					std::make_unique<GameState>(context)
				);
            });

	// ── 새 게임 버튼 ─────────────────────────────────────────────────────────
	uiManager.Add(
		std::make_unique<UIButton>(82, 32, 25, 3, 1,
			L("ui.new_game"),
			[this]()
			{
				context.sound.PlaySE("Assets/audio/ui_button_click.wav");

				// StoryState 생성
				auto story = std::make_unique<StoryState>(context, "prologue_000");

				// ── onInventory 콜백 연결 ────────────────────────────────────
				// raw 포인터를 람다에서 캡처합니다.
				// InventoryState 가 PopState 되면 StoryState 가 Resume() 됩니다.
				StoryState* storyPtr = story.get();
				story->onInventory = [this]()
					{
						context.PushState(
							std::make_unique<InventoryState>(context)
						);
					};

				context.ChangeState(std::move(story));
			})
	);

	uiManager.Add(
		std::make_unique<UIButton>(82, 37, 25, 3, 1,
			L("ui.load_game"),
			[this]()
			{
				context.sound.PlaySE("Assets/audio/ui_button_click.wav");
			})
	);

	uiManager.Add(
		std::make_unique<UIButton>(82, 42, 25, 3, 1,
			L("ui.setting"),
			[this]()
			{
				context.sound.PlaySE("Assets/audio/ui_button_click.wav");
				context.PushState(std::make_unique<SettingState>(context));
			})
	);

	uiManager.Add(
		std::make_unique<UIButton>(82, 47, 25, 3, 1,
			L("ui.quit_game"),
			[this]()
			{
				context.sound.PlaySE("Assets/audio/ui_button_click.wav");
				context.settingManager.Save("Data/settings.json");
				exit(0);
			})
	);

	uiManager.Add(
		std::make_unique<UIImage>(63, 3, 0, "Assets/ui/title.txt")
	);
}

void TitleState::HandleInput(InputManager& input)
{
	while ( input.HasAction() )
	{
		auto action = input.PopAction();

		switch ( action )
		{
		case InputAction::Confirm:
			context.eventBus.Emit(GameStartEvent{});
			break;

		case InputAction::Quit:
			exit(0);
			break;
		}
	}
}

void TitleState::Update()  {}

void TitleState::Render(ConsoleDisplay& display)
{
	uiManager.Render(display);
}
