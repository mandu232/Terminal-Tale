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
			button.Click();
		}
	}
}

void UIManager::Render()
{
	for ( const auto& button : buttons )
	{
		button.Render();
	}
}

void UIManager::Clear()
{
	buttons.clear();
}

void UIManager::HandleMouseMove(int x , int y)
{
	for ( auto& button : buttons )
	{
		button.SetHovered(button.Contains(x , y));
	}
}