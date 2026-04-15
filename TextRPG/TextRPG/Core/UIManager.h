#pragma once
#include <vector>
#include "UIButton.h"

class InputManager;

class UIManager
{
public:
	void AddButton(const UIButton& button);
	void HandleClick(int x , int y , InputManager& input);
	void HandleMouseMove(int x , int y);
	void Render();
	void Clear();

private:
	std::vector<UIButton> buttons;
};