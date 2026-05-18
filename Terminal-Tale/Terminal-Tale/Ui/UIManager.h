#pragma once
#include <vector>
#include <memory>
#include "UIElement.h"

class InputManager;
class ConsoleDisplay;

class UIManager
{
public:
	void Add(std::unique_ptr<UIElement> element);
	void HandleClick(int x , int y);
	void HandleMouseMove(int x , int y);
	void Render(ConsoleDisplay& display);
	void Clear();
	void Update(float fps);

private:
	std::vector<std::unique_ptr<UIElement>> elements;
};