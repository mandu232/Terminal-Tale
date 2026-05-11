#pragma once

#include "Game/Player/PlayerStatus.h"
#include "State.h"
#include "EventBus.h"
#include "StateMachine.h"
#include "SettingsManager.h"
#include "SoundSystem.h"
#include "ConsoleDisplay.h"
#include "LocalizationManager.h"

class Context
{
public:
	Context() : display(192 , 54) {};

	// ===== Core System =====

	EventBus eventBus;
	SettingsManager settingManager;
	SoundSystem sound;
	LocalizationManager localization;

	ConsoleDisplay display;

	std::unique_ptr<State> nextState;
	StateMachine* stateMachine = nullptr;

	PlayerStats player;

	std::unordered_set<std::string> flags;

	void ChangeState(std::unique_ptr<State> state);
	void PushState(std::unique_ptr<State> state);
	void PopState();

	void ToggleFullscreen(bool mode);
};