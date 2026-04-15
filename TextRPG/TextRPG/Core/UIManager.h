#pragma once
#include <vector>
#include "UIButton.h"

class InputManager;

class UIManager
{
public:
	void AddButton(const UIButton& button);
	void HandleClick(int x , int y , InputManager& input);
	void Render();

private:
	std::vector<UIButton> buttons;
};