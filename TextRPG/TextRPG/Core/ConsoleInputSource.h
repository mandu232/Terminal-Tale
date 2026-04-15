#pragma once

#include "InputSource.h"
#include <Windows.h>

class UIManager;

class ConsoleInputSource : public InputSource
{
public:
	ConsoleInputSource(UIManager& ui);

	void Update(InputManager& input) override;

private:
	HANDLE hInput;
	UIManager& uiManager;
};