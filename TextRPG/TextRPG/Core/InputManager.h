#pragma once 

#include "InputAction.h"
#include "InputSource.h"
#include <memory>
#include <queue>

class InputManager
{
public:
	void AddSource(std::unique_ptr<InputSource> source);
	void Update();

	void PushAction(InputAction action);

	bool HasAction() const;
	InputAction PopAction();

private:
	std::queue<InputAction> actionQueue;
	std::vector<std::unique_ptr<InputSource>> sources;
};