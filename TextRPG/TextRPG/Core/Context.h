#pragma once

#include "EventBus.h"
#include "StateMachine.h"

class Context
{
public:
	Context() = default;

	// ===== Core System =====

	EventBus eventBus;

	// ===== Game State =====

	std::unique_ptr<State> nextState;

	StateMachine* stateMachine = nullptr;

	void ChangeState(std::unique_ptr<State> state);
	void PushState(std::unique_ptr<State> state);
	void PopState();
};