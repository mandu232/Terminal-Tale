#pragma once

#include <Windows.h>

struct Resolution
{
	int width;
	int height;
};

class ResolutionManager
{
public:
	void Initialize(int w , int h);
	
	void SetResolution(int w , int h);
	const Resolution& Get() const;

	void ApplyToConsole();

private:
	Resolution current;

	HANDLE hConsole = nullptr;
};