#pragma once

#include "InputSource.h"
#include <Windows.h>

class UIManager;

class ConsoleInputSource : public InputSource
{
public:
	ConsoleInputSource();

	virtual void Update(InputManager& input, UIManager& ui) override;

private:
	HANDLE hInput;
};