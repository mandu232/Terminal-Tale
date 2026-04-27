#pragma once

#include "EventBus.h"
#include "UIManager.h"

class Context
{
public:
	Context() = default;

	// ===== Core System =====

	EventBus eventBus;

	// ===== Game State =====

	std::unique_ptr<State> nextState;

	void RequestStateChange(std::unique_ptr<State> state)
	{
		nextState = std::move(state);
	}
};