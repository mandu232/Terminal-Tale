#pragma once
#include <windows.h>
#include <vector>
#include <string>


enum ConsoleColor
{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	YELLOW = 6,
	WHITE = 7,
	BRIGHT_WHITE = 15
};

class ConsoleBuffer
{
public:
	ConsoleBuffer(int width , int height);

	void Clear(short color = 7);
	void Draw(int x , int y , char c , short color = 7);
	void DrawText(int x , int y , const std::string& text , short color = 7);

	void Present();

private:
	int width;
	int height;

	HANDLE hConsole;
	std::vector<CHAR_INFO> screen;
};