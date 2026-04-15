#pragma once

#include "EventBus.h"
#include "UIManager.h"

class Context
{
public:
	Context() = default;

	// ===== Core System =====

	EventBus eventBus;
	UIManager uiManager;

	// ===== Game State =====
};