#pragma once

class ConsoleBuffer;

class UIElement
{
public:
	virtual void Render(ConsoleBuffer& buffer) const = 0;

	virtual bool Contains(int x , int y) const { return false; };
	virtual void Click() {}
	virtual void SetHovered(bool) {}

	virtual int GetZ() const = 0;
	virtual ~UIElement() = default;
};