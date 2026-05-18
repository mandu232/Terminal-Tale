#include <Windows.h>
#include "EnableMouseInput.h"

void EnableMouseInput()
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	DWORD mode = 0;
	GetConsoleMode(hInput , &mode);

	mode |= ENABLE_MOUSE_INPUT;
	mode |= ENABLE_EXTENDED_FLAGS;
	mode &= ~ENABLE_QUICK_EDIT_MODE;

	SetConsoleMode(hInput , mode);
}