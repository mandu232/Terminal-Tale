#include "UIManager.h"
#include "InputManager.h"
#include "ConsoleBuffer.h"
#include <iostream>
#include <algorithm>

void UIManager::Add(std::unique_ptr<UIElement> element)
{
	elements.push_back(std::move(element));

	std::sort(
		elements.begin() ,
		elements.end() ,
		[] (const auto& a , const auto& b)
		{
			return a->GetZ() < b->GetZ();
		}
	);
}

void UIManager::HandleClick(int x , int y)
{
	for ( auto it = elements.rbegin(); it != elements.rend(); ++it )
	{
		if ( ( *it )->Contains(x , y) )
		{
			( *it )->Click();
			return;
		}
	}
}

void UIManager::Render(ConsoleBuffer& buffer)
{
	for ( auto& e : elements )
	{
		e->Render(buffer);
	}
}

void UIManager::Clear()
{
	elements.clear();
}

void UIManager::HandleMouseMove(int x , int y)
{
	for ( auto& e : elements )
	{
		e->SetHovered(e->Contains(x , y));
	}
}