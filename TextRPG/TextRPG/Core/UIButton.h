#pragma once
#include "InputAction.h"

class UIButton
{
public:
	UIButton(int x , int y , int w , int h , InputAction action);

	bool Contains(int mx , int my) const;
	InputAction GetAction() const;

	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;

private:
	int x , y , width , height;
	InputAction action;
};