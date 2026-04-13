#include "InputManager.h"
#include <iostream>

void InputManager::Update()
{
	currentAction = InputAction::None;

	char input;
	std::cin >> input;

    switch (input)
    {
    case 'w': currentAction = InputAction::MoveUp; break;
    case 's': currentAction = InputAction::MoveDown; break;
    case 'a': currentAction = InputAction::MoveLeft; break;
    case 'd': currentAction = InputAction::MoveRight; break;
    case 'e': currentAction = InputAction::Confirm; break;
    case 'q': currentAction = InputAction::Quit; break;
    }
}

InputAction InputManager::GetAction() const
{
    return currentAction;
}