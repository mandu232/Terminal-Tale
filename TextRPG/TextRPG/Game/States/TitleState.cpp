#include "TitleState.h"
#include "GameState.h"
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Game/Events/GameStartEvent.h"
#include "Core/ConsoleBuffer.h"
#include <iostream>

TitleState::TitleState(Context& context)
    : State(context)
{
}

void TitleState::Enter()
{
    startSub =
        context.eventBus.Subscribe<GameStartEvent>(
            [this](const GameStartEvent&)
            {
				context.RequestStateChange(
					std::make_unique<GameState>(context)
				);
            });

	context.uiManager.AddButton
	(
		UIButton(10 , 5 , 20 , 3 , 1, 
			"Start Game",
			[this]() {
			context.eventBus.Emit(GameStartEvent{});
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
	context.uiManager.Render(buffer);

	if ( receivedStartEvent )
	{
		buffer.DrawText(10 , 10 , "Game Start Event Receive", 4);
	}
}