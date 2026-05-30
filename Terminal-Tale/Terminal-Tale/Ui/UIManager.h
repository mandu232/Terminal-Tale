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
	void Clear();      // 즉시 전체 제거
	void SoftClear();  // 퇴장 애니메이션 후 자동 제거 (IsDead() 기반)
	void Update(float fps);

private:
	std::vector<std::unique_ptr<UIElement>> elements;
};