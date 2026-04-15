#include "UIManager.h"
#include "InputManager.h"
#include <iostream>

void UIManager::AddButton(const UIButton& button)
{
	buttons.push_back(button);
}

void UIManager::HandleClick(int x , int y , InputManager& input)
{
	for ( auto& button : buttons )
	{
		if ( button.Contains(x , y) )
		{
			input.PushAction(button.GetAction());
		}
	}
}

void UIManager::Render()
{
	for ( const auto& button : buttons )
	{
		int x = button.GetX();
		int y = button.GetY();
		int w = button.GetWidth();
		int h = button.GetHeight();

		for ( int i = 0; i < h; ++i )
		{
			std::cout << "\033[" << ( y + i ) << ";" << x << "H";

			for ( int j = 0; j < w; ++j )
				std::cout << '#';
		}
	}
}