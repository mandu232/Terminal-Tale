#include "InputManager.h"


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
void InputManager::AddSource(std::unique_ptr<InputSource> source)
{
	sources.push_back(std::move(source));
}