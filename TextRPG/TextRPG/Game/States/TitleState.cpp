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
}

void TitleState::HandleInput(InputManager& input)
{
    switch (input.GetAction())
    {
    case InputAction::Confirm:
        std::cout << "Start Game\n";
        context.eventBus.Emit(GameStartEvent{});
        break;

    case InputAction::Quit:
        exit(0);
        break;
    }
}

void TitleState::Update()
{
}

void TitleState::Render()
{
}