#include "TitleState.h"
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Game/Events/GameStartEvent.h"
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
                std::cout << "Game Start Event Received\n";
            });
	context.uiManager.AddButton
	(
		UIButton(10 , 5 , 20 , 3 , [this]() {
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

void TitleState::Render()
{
	context.uiManager.Render();
}