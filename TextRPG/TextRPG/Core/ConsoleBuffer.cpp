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
	const std::wstring& text ,
	short color)
{
	int currentX = x;

	for ( wchar_t c : text )
	{
		if ( currentX >= width ) break;

		Draw(currentX , y , c , color);

		if ( c >= 0x1100 )
			currentX += 2;
		else
			currentX += 1;
	}
}

void ConsoleBuffer::DrawText(
	int x ,
	int y ,
	const std::string& text ,
	short color)
{
	DrawText(x , y , UTF8ToWide(text) , color);
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