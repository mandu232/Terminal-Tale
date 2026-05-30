#include "UIManager.h"
#include "Core/InputManager.h"
#include "Core/ConsoleDisplay.h"
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

void UIManager::Render(ConsoleDisplay& display)
{
	for ( auto& e : elements )
	{
		e->Render(display);
	}
}

void UIManager::Clear()
{
	elements.clear();
}

// ─────────────────────────────────────────────
//  SoftClear — 모든 요소에 SlideOut() 을 호출한다.
//
//  UIElement 기본 구현: SlideOut() → m_dead = true (즉시 dead)
//  UIDocumentPanel   : SlideOut() → 아래 슬라이드 애니메이션 시작
//
//  죽은 요소는 Update() 루프 끝에서 자동 제거된다.
// ─────────────────────────────────────────────
void UIManager::SoftClear()
{
	for ( auto& e : elements )
		e->SlideOut();
}

void UIManager::HandleMouseMove(int x , int y)
{
	for ( auto& e : elements )
	{
		e->SetHovered(e->Contains(x , y));
	}
}

void UIManager::Update(float fps)
{
	if ( fps <= 0.f ) return;

	const float deltaTime = 1.0f / fps;

	for ( auto& e : elements )
		e->Update(deltaTime);

	// IsDead() == true 인 요소 제거 (SlideOut 완료 포함)
	elements.erase(
		std::remove_if(elements.begin() , elements.end() ,
			[] (const auto& e) { return e->IsDead(); }) ,
		elements.end());
}