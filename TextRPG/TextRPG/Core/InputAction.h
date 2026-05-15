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
};