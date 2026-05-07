#pragma once

class ConsoleDisplay;

class UIElement
{
public:
	virtual void Render(ConsoleDisplay& display) const = 0;

	// 애니메이션 등 프레임마다 호출 (기본 구현은 아무것도 안 함)
	virtual void Update(float deltaTime) {}

	virtual bool Contains(int x , int y) const { return false; };
	virtual void Click() {}
	virtual void SetHovered(bool) {}

	virtual int GetZ() const = 0;
	virtual ~UIElement() = default;
};
