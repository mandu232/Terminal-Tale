#include "TitleState.h"
#include "GameState.h"
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Game/Events/GameStartEvent.h"
#include "Game/Events/PlaySoundEvent.h"
#include "Core/ConsoleDisplay.h"
#include <iostream>
#include <Core/Localization.h>
#include "Core/UIButton.h"
#include "Core/UILabel.h"

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
		std::make_unique<UIButton>(10 , 30 , 20 , 3 , 1 ,
			L("ui.new_game") ,
			[ this ] () {
				context.sound.PlaySE("Assets/audio/testsound.wav");
			})
	);

	uiManager.Add
	(
		std::make_unique<UIButton>(10 , 35 , 20 , 3 , 1, 
			L("ui.load_game"),
			[this]() {
			context.sound.PlaySE("Assets/audio/testsound.wav");
			context.eventBus.Emit(GameStartEvent{});
		})
	);

	uiManager.Add
	(
		std::make_unique<UIButton>(10 , 40 , 20 , 3 , 1 ,
			L("ui.setting") ,
			[ this ] () {
				context.sound.PlaySE("Assets/audio/testsound.wav");
				context.eventBus.Emit(GameStartEvent{});
			})
	);

	uiManager.Add
	(
		std::make_unique<UIButton>(10 , 45 , 20 , 3 , 1 ,
			L("ui.quit_game"),
			[ this ] () {
				//테스트용 임시 저장
				context.settings.Save("Data/settings.json");
				exit(0);
			})
	);

	uiManager.Add
	(
		std::make_unique<UILabel>(50 , 15 , 10, 20 , L("ui.test_text") , 7)
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