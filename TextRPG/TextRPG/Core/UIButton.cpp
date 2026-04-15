#include "UIButton.h"

UIButton::UIButton(int x , int y , int w , int h , InputAction action)
	: x(x) , y(y) , width(w) , height(h) , action(action) {
}

bool UIButton::Contains(int mx , int my) const
{
	return mx >= x &&
		mx <= x + width &&
		my >= y &&
		my <= y + height;
}

InputAction UIButton::GetAction() const
{
	return action;
}
int UIButton::GetX() const { return x; }
int UIButton::GetY() const { return y; }
int UIButton::GetWidth() const { return width; }
int UIButton::GetHeight() const { return height; }