#include "StateMachine.h"
#include "State.h"

void StateMachine::ChangeState(std::unique_ptr<State> newState)
{
	if (currentState)
	{
		currentState->Exit();
	}

	currentState = std::move(newState);

	if (currentState)
	{
		currentState->Enter();
	}
}

void StateMachine::HandleInput()
{
	if (currentState)
	{
		currentState->HandleInput();
	}
}

void StateMachine::Update()
{
	if (currentState)
	{
		currentState->Update();
	}
}

void StateMachine::Render()
{
	if (currentState)
	{
		currentState->Render();
	}
}