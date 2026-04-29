#pragma once
#include <vector>
#include <memory>
#include "UIElement.h"

class InputManager;
class ConsoleBuffer;

class UIManager
{
public:
	void Add(std::unique_ptr<UIElement> element);
	void HandleClick(int x , int y);
	void HandleMouseMove(int x , int y);
	void Render(ConsoleBuffer& buffer);
	void Clear();

private:
	std::vector<std::unique_ptr<UIElement>> elements;
};