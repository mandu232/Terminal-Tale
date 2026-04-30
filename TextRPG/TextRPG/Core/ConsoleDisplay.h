#pragma once
#include <Windows.h>
#include <vector>

class ConsoleDisplay
{
private:

	HANDLE hConsole;
	int width;
	int height;
	std::vector<CHAR_INFO> screen;

	LONG savedStyle;
	RECT savedWindowRect;

public:

	ConsoleDisplay(int w , int h);
	void Resize(int w , int h);
	void SetFullscreen(bool enable);
	void AdaptFontSizeToWindow();
	void Clear(short color);
	void Draw(int x , int y , wchar_t c , short color);
	void Present();
};