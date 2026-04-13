#include "TitleState.h"
#include <iostream>

void TitleState::Enter()
{
	std::cout << "Welcome to the Text RPG!" << std::endl;
}

void TitleState::HandleInput()
{
	std::cout << "Press Enter to start the game..." << std::endl;
	std::cin.get();
}

void TitleState::Update()
{
}

void TitleState::Render()
{
}