#pragma once

#include "InputSource.h"
#include "SettingsManager.h"
#include <Windows.h>

class UIManager;

class ConsoleInputSource : public InputSource
{
public:
	explicit ConsoleInputSource(const Settings* settings = nullptr);

	virtual void Update(InputManager& input, UIManager& ui) override;

private:
	HANDLE hInput;
	bool isLeftPressedBefore = false;
	const Settings* settings_ = nullptr;
};