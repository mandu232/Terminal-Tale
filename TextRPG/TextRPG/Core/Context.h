#pragma once

#include "EventBus.h"
#include "StateMachine.h"
#include "SettingsManager.h"
#include "SoundSystem.h"

class Context
{
public:
	Context() = default;

	// ===== Core System =====

	EventBus eventBus;
	SettingsManager settings;
	SoundSystem sound;

	// ===== Game State =====

	std::unique_ptr<State> nextState;

	StateMachine* stateMachine = nullptr;

	void ChangeState(std::unique_ptr<State> state);
	void PushState(std::unique_ptr<State> state);
	void PopState();
};