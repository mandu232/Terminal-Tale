#pragma once

#include <memory>
#include <stack>

class State;
class InputManager;
class Context;
class ConsoleDisplay;

class StateMachine
{
public:
	void ChangeState(std::unique_ptr<State> newState);
	void PushState(std::unique_ptr<State> newState);
	void PopState();

	void HandleInput(InputManager& input);
	void Update();
	void Render(ConsoleDisplay& display);

	StateMachine(Context& context);

	State* GetcurrentState() const;

private:
	std::stack<std::unique_ptr<State>> stateStack;
	Context& context;
};