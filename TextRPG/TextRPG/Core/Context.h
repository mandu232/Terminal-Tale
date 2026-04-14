#pragma once

#include "EventBus.h"

class Context
{
public:
	Context() = default;

	// ===== Core System =====

	EventBus eventBus;

	// ===== Game State =====
};