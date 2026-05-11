#include "TitleState.h"
#include "GameState.h"
#include "StoryState.h"
#include "SettingState.h"
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Game/Events/GameStartEvent.h"
#include "Game/Events/PlaySoundEvent.h"
#include "Core/ConsoleDisplay.h"
#include <iostream>
#include <Core/Localization.h>
#include "Core/UIButton.h"
#include "Core/UILabel.h"
#include "Core/UIImage.h"

TitleState::TitleState(Context& context)
    : State(context)
{
}

void TitleState::Enter()
{

	playSound =
		context.eventBus.Subscribe<PlaySoundEvent>(
			[ this ] (const PlaySoundEvent& e)
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

	uiManager.Add
	(
		std::make_unique<UIButton>(82 , 32 , 25 , 3 , 1 ,
			L("ui.new_game") ,
			[ this ] () {
				context.sound.PlaySE("Assets/audio/ui_button_click.wav");
				context.ChangeState(std::make_unique<StoryState>(context , "prologue_000"));
			})
	);

	uiManager.Add
	(
		std::make_unique<UIButton>(82 , 37 , 25 , 3 , 1, 
			L("ui.load_game"),
			[this]() {
			context.sound.PlaySE("Assets/audio/ui_button_click.wav");
		})
	);

	uiManager.Add
	(
		std::make_unique<UIButton>(82 , 42 , 25 , 3 , 1 ,
			L("ui.setting") ,
			[ this ] () {
				context.sound.PlaySE("Assets/audio/ui_button_click.wav");
				context.PushState(std::make_unique<SettingState>(context));
			})
	);

	uiManager.Add
	(
		std::make_unique<UIButton>(82 , 47 , 25 , 3 , 1 ,
			L("ui.quit_game"),
			[ this ] () {
				//테스트용 임시 저장
				context.sound.PlaySE("Assets/audio/ui_button_click.wav");
				context.settingManager.Save("Data/settings.json");
				exit(0);
			})
	);
	//타이틀 이미지 
	uiManager.Add
	(
		std::make_unique<UIImage>(
			63, 3, 0,
			"Assets/ui/title.txt"
		)
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

void TitleState::Update()
{
}

void TitleState::Render(ConsoleDisplay& display)
{
	uiManager.Render(display);
}