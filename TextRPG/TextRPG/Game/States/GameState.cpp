#include "GameState.h"
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/UIButton.h"
#include <iostream>

GameState::GameState(Context& context)
	: State(context)
{
}

void GameState::Enter()
{
	uiManager.Add
	(
		std::make_unique<UIButton>(20 , 5 , 20 , 3 , 1 ,
			"GameState" ,
			[ this ] () {
				context.PopState();
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

void GameState::Render(ConsoleDisplay& display)
{
	uiManager.Render(display);

}