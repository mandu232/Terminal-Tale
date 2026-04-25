#pragma once
#include <vector>
#include "UIButton.h"

class InputManager;
class ConsoleBuffer;

class UIManager
{
public:
	void AddButton(const UIButton& button);
	void HandleClick(int x , int y);
	void HandleMouseMove(int x , int y);
	void Render(ConsoleBuffer& buffer);
	void Clear();

private:
	std::vector<UIButton> buttons;
};