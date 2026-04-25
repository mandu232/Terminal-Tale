#pragma once
#include <functional>
#include <string>

class ConsoleBuffer;

class UIButton
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

	int GetZ() const { return zOreder; }

	bool Contains(int mx , int my) const;
	void Click();

	void SetHovered(bool value)
	{
		state = value ? State::Hovered : State::Normal;
	}

	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;

	void Render(ConsoleBuffer& buffer) const;
	short GetColor() const;

private:
	int x , y , width , height;
	int zOreder = 0;
	State state = State::Normal;
	std::string text;
	Callback onClick;
};