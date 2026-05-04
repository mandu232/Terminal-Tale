#include "Core/Application.h"
#include "Utils/EnableMouseInput.h"
#include <Windows.h>



int main()
{
	EnableMouseInput();
	SetConsoleOutputCP(65001);

	Application app;
	app.Run();

	return 0;
}