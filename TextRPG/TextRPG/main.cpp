#include "Core/Application.h"
#include <Windows.h>
#include <iostream>

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

int main()
{
	EnableMouseInput();
	SetConsoleOutputCP(65001);

	Application app;
	app.Run();

	return 0;
}