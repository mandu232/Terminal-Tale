#pragma once

#include "Game/Player/PlayerStatus.h"
#include "Game/Log/LogEntry.h"
#include "Game/Journal/JournalEntry.h"
#include "State.h"
#include "EventBus.h"
#include "StateMachine.h"
#include "SettingsManager.h"
#include "SoundSystem.h"
#include "ConsoleDisplay.h"
#include "LocalizationManager.h"

#include <vector>
#include <string>

class Context
{
public:
	Context() : display(192 , 54) {};

	// ===== Core Systems =====
	EventBus eventBus;
	SettingsManager settingManager;
	SoundSystem sound;
	LocalizationManager localization;
	ConsoleDisplay display;

	std::unique_ptr<State> nextState;
	StateMachine* stateMachine = nullptr;

	// ===== Game State =====
	PlayerStats player;
	std::unordered_set<std::string> flags;
	std::string currentNodeId;
	int activeSlot = 0;  // 0 = 미선택, 1-3 = 슬롯

	// ===== Log & Journal =====
	std::vector<LogEntry> log;
	std::vector<JournalEntry> journal;

	// 로그 항목 추가 (day/time은 현재 player 값 자동 사용)
	void AddLog(const std::string& text);

	// 저널 항목 추가 — 같은 id가 있으면 내용을 덮어씀
	void AddJournal(const std::string& id,
	                const std::string& title,
	                const std::string& content);

	// ===== Save / Load =====
	struct SlotInfo
	{
		bool exists = false;
		int day = 0;
		int time = 0;
		std::string nodeId;
	};

	bool SaveSlot(int slot) const;
	bool LoadSlot(int slot);
	bool HasSaveSlot(int slot) const;
	SlotInfo GetSlotInfo(int slot) const;

	// 새 게임 시작 시 게임 상태 전체 초기화
	void ResetGameState();

	// ===== State Machine Helpers =====
	void ChangeState(std::unique_ptr<State> state);
	void PushState(std::unique_ptr<State> state);
	void PopState();

	void ToggleFullscreen(bool mode);

private:
	static std::string SlotPath(int slot);
};