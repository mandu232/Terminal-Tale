#pragma once 

enum class InputAction
{
	None,

	Confirm,
	Cancel,

	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,

	OpenMenu,
	Quit,

	Click,

	// 단축키
	OpenInventory,   // 'I' — 인벤토리 열기
	OpenWait,        // 'Z' — 대기
	OpenSleep,       // 'S' — 수면
	OpenLog,         // 'L' — 기록
	OpenJournal,     // 'J' — 저널
	QuickSave,       // F5  — 빠른 저장
	QuickLoad,       // F9  — 빠른 불러오기

	// 마우스 휠
	ScrollUp,
	ScrollDown,
};