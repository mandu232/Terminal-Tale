#include "StateMachine.h"
#include "InputManager.h"
#include "Context.h"
#include "State.h"

void StateMachine::ChangeState(std::unique_ptr<State> newState)
{
	while ( !stateStack.empty() )
	{
		stateStack.top()->Exit();
		stateStack.pop();
	}
	PushState(std::move(newState));
}

void StateMachine::HandleInput(InputManager& input)
{
	if (!stateStack.empty())
	{
		stateStack.top()->HandleInput(input);
	}
}

void StateMachine::Update()
{
	if (!stateStack.empty())
	{
		stateStack.top()->Update();
	}
}

void StateMachine::Render(ConsoleBuffer& buffer)
{
	if (!stateStack.empty())
	{
		stateStack.top()->Render(buffer);
	}
}

State* StateMachine::GetcurrentState() const
{
	return stateStack.empty() ? nullptr : stateStack.top().get();
}

void StateMachine::PushState(std::unique_ptr<State> newState)
{
	if ( newState )
	{
		stateStack.push(std::move(newState));
		stateStack.top()->Enter();

	}
}

void StateMachine::PopState()
{
	if ( !stateStack.empty() )
	{
		stateStack.top()->Exit();
		stateStack.pop();
	}
	if ( !stateStack.empty() )
	{
		stateStack.top()->Resume();
	}
}

StateMachine::StateMachine(Context& context) 
	: context(context)
{
}