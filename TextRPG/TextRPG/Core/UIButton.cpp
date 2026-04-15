#include "UIButton.h"
#include <iostream>

UIButton::UIButton(int x , int y , int w , int h , Callback onClick)
	: x(x) , y(y) , width(w) , height(h) , onClick(onClick) {
}

bool UIButton::Contains(int mx , int my) const
{
	return mx >= x &&
		mx <= x + width &&
		my >= y &&
		my <= y + height;
}

void UIButton::Click()
{
	if ( onClick )
		onClick();
}

int UIButton::GetX() const { return x; }
int UIButton::GetY() const { return y; }
int UIButton::GetWidth() const { return width; }
int UIButton::GetHeight() const { return height; }

void UIButton::Render() const
{
	for ( int i = 0; i < height; ++i )
	{
		std::cout << "\033[" << ( y + i ) << ";" << x << "H";

		for ( int j = 0; j < width; ++j )
		{
			std::cout << GetColorCode();
		}
	}
}

const char* UIButton::GetColorCode() const
{
	switch ( state )
	{
	case State::Normal:  return "\033[37m";
	case State::Hovered: return "\033[33m";
	case State::Pressed: return "\033[32m";
	}
	return "\033[37m";
}