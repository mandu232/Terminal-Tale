#include "UIButton.h"
#include "ConsoleDisplay.h"
#include "Utils/GetVisualWidth.h"
#include "Utils/UTF8ToWide.h"
#include <iostream>

UIButton::UIButton(int x , int y , int w , int h ,
	int z,
	std::string textStr,
	Callback onClick)
	: x(x) , y(y) , width(w) , height(h) ,
	zOreder(z),
	onClick(onClick) 
{
	this->text = UTF8ToWide(textStr);
}

bool UIButton::Contains(int mx , int my) const
{
	return mx >= x &&
		mx < x + width &&
		my >= y &&
		my < y + height;
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

void UIButton::Render(ConsoleDisplay& display) const 
{
	short color = GetColor();

	for ( int i = 0; i < height; i++ ) {
		for ( int j = 0; j < width; j++ ) {


			wchar_t c = L' ';

			if ( i == 0 && j == 0 ) c = L'+';
			else if ( i == 0 && j == width - 1 ) c = L'+';
			else if ( i == height - 1 && j == 0 ) c = L'+';
			else if ( i == height - 1 && j == width - 1 ) c = L'+';
			else if ( i == 0 || i == height - 1 ) c = L'-';
			else if ( j == 0 || j == width - 1 ) c = L'|';


			display.Draw(x + j , y + i , c , color);
		}
	}

	int visualWidth = GetVisualWidth(text);
	int textX = x + ( width - visualWidth ) / 2;
	int textY = y + height / 2;

	display.DrawText(textX , textY , text , color);
}

short UIButton::GetColor() const
{
	switch ( state )
	{
	case State::Normal:  return 7;   // WHITE
	case State::Hovered: return 14;  // YELLOW
	case State::Pressed: return 10;  // GREEN
	}

	return 7;
}