#pragma once 

#include "InputAction.h"

class InputManager
{
public:
	void Update();

	InputAction GetAction() const;

private:
	InputAction currentAction = InputAction::None;
};