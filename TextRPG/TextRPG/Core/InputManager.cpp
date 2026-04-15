#include "InputManager.h"
#include "MouseInput.h"
#include "KeyboardInput.h"


void InputManager::PushAction(InputAction action)
{
	actionQueue.push(action);
}

bool InputManager::HasAction() const
{
	return !actionQueue.empty();
}

InputAction InputManager::PopAction()
{
	auto action = actionQueue.front();
	actionQueue.pop();
	return action;
}
void InputManager::Update()
{
	for ( auto& source : sources )
	{
		source->Update(*this);
	}
}
InputManager::InputManager()
{
	sources.push_back(std::make_unique<KeyboardInput>());
	sources.push_back(std::make_unique<MouseInput>());
}