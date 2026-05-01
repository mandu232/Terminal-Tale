#pragma once
#include <functional>
#include <string>
#include "UIElement.h"

class ConsoleDisplay;

class UIButton :public UIElement
{
public:
	enum class State
	{
		Normal ,
		Hovered ,
		Pressed
	};

	using Callback = std::function<void()>;

	UIButton(int x , int y , int w , int h ,
		int z,
		std::string text,
		Callback onClick);

	int GetZ() const override { return zOreder; }

	bool Contains(int mx , int my) const override;
	void Click() override;

	void SetHovered(bool value) override
	{
		state = value ? State::Hovered : State::Normal;
	}

	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;

	void Render(ConsoleDisplay& display) const override;
	short GetColor() const;

private:
	int x , y , width , height;
	int zOreder = 0;
	State state = State::Normal;
	std::wstring text;
	Callback onClick;
};