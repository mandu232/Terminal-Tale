#include "UIButton.h"
#include "ConsoleBuffer.h"
#include "Utils/GetVisualWidth.h"
#include <iostream>

UIButton::UIButton(int x , int y , int w , int h ,
	int z,
	std::string text,
	Callback onClick)
	: x(x) , y(y) , width(w) , height(h) ,
	zOreder(z),
	text(std::move(text)),
	onClick(onClick) {
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

void UIButton::Render(ConsoleBuffer& buffer) const
{
	short color = GetColor();

	//배경 출력
	for ( int i = 0;i < height;i++ )
	{
		for ( int j = 0;j < width;j++ )
		{
			wchar_t c = L' ';

			if ( i == 0 || i == height - 1 ) c = L'-';
			else if ( j == 0 || j == width - 1 ) c = L'|';

			buffer.Draw(x + j , y + i , c, color);
		}
	}

	int visualWidth = GetVisualWidth(text);
	int textX = x + ( width - visualWidth ) / 2;
	int textY = y + height / 2;

	buffer.DrawText(textX , textY , text, color);
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