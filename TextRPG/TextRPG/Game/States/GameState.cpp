#include "GameState.h"
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Game/Events/GameStartEvent.h"
#include "Core/ConsoleBuffer.h"
#include <iostream>

GameState::GameState(Context& context)
	: State(context)
{
}

void GameState::Enter()
{
	context.uiManager.AddButton
	(
		UIButton(20 , 5 , 20 , 3 , 1 ,
			"GameState" ,
			[ this ] () {
				context.eventBus.Emit(GameStartEvent{});
			})
	);

}

void GameState::HandleInput(InputManager& input)
{
	while ( input.HasAction() )
	{
		auto action = input.PopAction();
	}
}

void GameState::Update()
{
}

void GameState::Render(ConsoleBuffer& buffer)
{
	context.uiManager.Render(buffer);

}