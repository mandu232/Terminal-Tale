#pragma once
#include <functional>

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

	UIButton(int x , int y , int w , int h , Callback onClick);

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

	void Render() const;
	const char* GetColorCode() const;

private:
	int x , y , width , height;
	State state = State::Normal;
	Callback onClick;
};