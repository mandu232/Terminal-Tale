#include "TitleState.h"
#include "Core/InputManager.h"
#include <iostream>

void TitleState::Enter()
{
	std::cout << "Welcome to the Text RPG!" << std::endl;
}

void TitleState::HandleInput(InputManager& input)
{
    switch (input.GetAction())
    {
    case InputAction::Confirm:
        std::cout << "Start Game\n";
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