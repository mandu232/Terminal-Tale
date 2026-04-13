#pragma once

#include <memory>

class State;

class StateMachine
{
public:
	void ChangeState(std::unique_ptr<State> newState);

	void HandleInput();
	void Update();
	void Render();

private:
	std::unique_ptr<State> currentState;
};