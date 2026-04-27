#pragma once

#include <memory>
#include <stack>

class State;
class InputManager;
class Context;
class ConsoleBuffer;

class StateMachine
{
public:
	void ChangeState(std::unique_ptr<State> newState);
	void PushState(std::unique_ptr<State> newState);
	void PopState();

	void HandleInput(InputManager& input);
	void Update();
	void Render(ConsoleBuffer& buffer);

	StateMachine(Context& context);

	State* GetcurrentState() const;

private:
	std::stack<std::unique_ptr<State>> stateStack;
	Context& context;
};