#include "ConsoleBuffer.h"
#include "Utils/UTF8ToWide.h"
#include <iostream>

ConsoleBuffer::ConsoleBuffer(int w , int h)
	: width(w) , height(h)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	screen.resize(width * height);
}

void ConsoleBuffer::Clear(short color)
{
	for ( auto& cell : screen )
	{
		cell.Char.UnicodeChar = ' ';
		cell.Attributes = color;
	}
}

void ConsoleBuffer::Draw(int x , int y , wchar_t c , short color)
{
	if ( x < 0 || y < 0 || x >= width || y >= height )
		return;

	int index = y * width + x;

	screen[ index ].Char.UnicodeChar = c;
	screen[ index ].Attributes = color;
}

void ConsoleBuffer::DrawText(
	int x ,
	int y ,
	const std::string& text ,
	short color)
{
	std::wstring wtext = UTF8ToWide(text);
	int currentX = x;

	for ( wchar_t c : wtext ) {
		// 범위를 벗어나면 그리지 않음
		if ( currentX >= width ) break;

		// 1. 현재 위치에 문자 그리기
		Draw(currentX , y , c , color);

		// 2. 문자 너비 판별 (한글/특수문자 등은 보통 0x1100 이상)
		if ( c >= 0x1100 ) {
			// 한글일 경우: 2칸 이동
			// 주의: 한글 뒷부분(11번 칸)을 비워줘야 다음 글자가 덮어쓰지 않습니다.
			// 이미 Draw에서 10번 칸에 '가'를 넣었으므로, 11번 칸은 건너뜁니다.
			currentX += 2;
		}
		else {
			// 영어/숫자일 경우: 1칸 이동
			currentX += 1;
		}
	}
}

void ConsoleBuffer::Present()
{
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = width - 1;
	rect.Bottom = height - 1;

	COORD size = { ( SHORT )width, ( SHORT )height };
	COORD pos = { 0,0 };

	WriteConsoleOutputW(
		hConsole ,
		screen.data() ,
		size ,
		pos ,
		&rect
	);
}