#include "TitleState.h"
#include "GameState.h"
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Game/Events/GameStartEvent.h"
#include "Game/Events/PlaySoundEvent.h"
#include "Core/ConsoleBuffer.h"
#include <iostream>
#include <Core/Localization.h>

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

	uiManager.AddButton
	(
		UIButton(10 , 5 , 20 , 3 , 1, 
			L("ui.start_game") ,
			[this]() {
			context.sound.PlaySE("Assets/audio/testsound.wav");
			context.eventBus.Emit(GameStartEvent{});
		})
	);

	uiManager.AddButton
	(
		UIButton(10 , 20 , 20 , 3 , 1 ,
			"Quit" ,
			[ this ] () {
				//테스트용 임시 저장
				context.settings.Save("Data/settings.json");
				exit(0);
			})
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

void TitleState::Render(ConsoleBuffer& buffer)
{
	uiManager.Render(buffer);

	if ( receivedStartEvent )
	{
		buffer.DrawText(10 , 10 , "Game Start Event Receive", 4);
	}
}