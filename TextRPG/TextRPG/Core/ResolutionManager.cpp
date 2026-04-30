#include "ResolutionManager.h"

void ResolutionManager::Initialize(int w , int h)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetResolution(w , h);
}

void ResolutionManager::ApplyToConsole()
{
	COORD size{
		( SHORT )current.width,
		( SHORT )current.height };

	SetConsoleScreenBufferSize(hConsole , size);

	SMALL_RECT rect{
		0,
		0,
		( SHORT )( current.width - 1 ),
		( SHORT )( current.height - 1 )
	};

	SetConsoleWindowInfo(hConsole , TRUE , &rect);
}

void ResolutionManager::SetResolution(int w , int h)
{
	current.width = w;
	current.height = h;

	ApplyToConsole();
}

const Resolution& ResolutionManager::Get() const
{
	return current;
}