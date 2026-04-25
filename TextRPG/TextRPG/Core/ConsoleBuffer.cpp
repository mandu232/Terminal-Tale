#include "ConsoleBuffer.h"
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
		cell.Char.AsciiChar = ' ';
		cell.Attributes = color;
	}
}

void ConsoleBuffer::Draw(int x , int y , char c , short color)
{
	if ( x < 0 || y < 0 || x >= width || y >= height )
		return;

	int index = y * width + x;

	screen[ index ].Char.AsciiChar = c;
	screen[ index ].Attributes = color;
}

void ConsoleBuffer::DrawText(
	int x ,
	int y ,
	const std::string& text ,
	short color)
{
	for ( size_t i = 0; i < text.size(); ++i )
		Draw(x + ( int )i , y , text[ i ] , color);
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

	WriteConsoleOutputA(
		hConsole ,
		screen.data() ,
		size ,
		pos ,
		&rect
	);
}