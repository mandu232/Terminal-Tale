#include "Context.h"
#include "State.h"
#include "StateMachine.h"

void Context::ChangeState(std::unique_ptr<State> state)
{
	if ( stateMachine )
	{
		stateMachine->ChangeState(std::move(state));
	}
}

void Context::PushState(std::unique_ptr<State> state)
{
	if ( stateMachine )
	{
		stateMachine->PushState(std::move(state));
	}
}

void Context::PopState()
{
	if ( stateMachine )
	{
		stateMachine->PopState();
	}
}

void Context::ToggleFullscreen(bool mode)
{
	display.SetFullscreen(mode);
}