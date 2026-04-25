#include "UIManager.h"
#include "InputManager.h"
#include "ConsoleBuffer.h"
#include <iostream>
#include <algorithm>

void UIManager::AddButton(const UIButton& button)
{
	buttons.push_back(button);

	std::sort(
		buttons.begin() ,
		buttons.end() ,
		[] (const UIButton& a , const UIButton& b)
		{
			return a.GetZ() < b.GetZ();
		}
	);
}

void UIManager::HandleClick(int x , int y)
{
	for ( auto it = buttons.rbegin();
		it != buttons.rend();
		++it )
	{
		if ( it->Contains(x , y) )
		{
			it->Click();
			return;
		}
	}
}

void UIManager::Render(ConsoleBuffer& buffer)
{
	for ( const auto& button : buttons )
	{
		button.Render(buffer);
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