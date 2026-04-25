#pragma once

#include <memory>

class State;
class InputManager;
class Context;
class ConsoleBuffer;

class StateMachine
{
public:
	void ChangeState(std::unique_ptr<State> newState);

	void HandleInput(InputManager& input);
	void Update();
	void Render(ConsoleBuffer& buffer);

	StateMachine(Context& context);

private:
	std::unique_ptr<State> currentState;
	Context& context;
};