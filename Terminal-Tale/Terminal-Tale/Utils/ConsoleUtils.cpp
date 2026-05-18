#include <windows.h>

void HideCursor()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(console , &info);

	info.bVisible = FALSE;
	SetConsoleCursorInfo(console , &info);
}

void ShowCursor()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(console , &info);

	info.bVisible = TRUE;
	SetConsoleCursorInfo(console , &info);
}

void SetupConsole()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD mode = 0;
	GetConsoleMode(hOut , &mode);
	mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;
	SetConsoleMode(hOut , mode);
}