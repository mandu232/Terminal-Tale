#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class ConsoleDisplay
{
private:

	HANDLE hConsole;
	int width;
	int height;
	std::vector<CHAR_INFO> screen;

	// 창 복구를 위한 초기 스타일 저장용
	LONG savedStyle;
	RECT savedWindowRect;

public:

	ConsoleDisplay(int w , int h);

	//해상도 및 창 관리
	void Resize(int w , int h);
	void SetFullscreen(bool enable);
	void AdaptFontSizeToWindow();

	//그리기 기능
	void Clear(short color = 0);
	void Draw(int x , int y , wchar_t c , short color);
	void DrawText(int x , int y , const std::wstring& text , short color);
	void Present();

	//게터
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
};