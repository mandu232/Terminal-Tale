#pragma once
#include "UIElement.h"

// ─────────────────────────────────────────────
//  UIScreenFader
//
//  화면 전체를 검정으로 덮었다가 위→아래 방향으로 걷어 내는
//  페이드인 효과.  StateMachine 이 PushState / PopState 직후
//  자동으로 현재 State 의 UIManager 에 추가한다.
//
//  progress 0 → 화면 전체 검정
//  progress 1 → 완전히 투명 (IsDead() = true, 자동 제거)
//
//  GetZ() = 9999 로 모든 UI 요소 위에 렌더링된다.
// ─────────────────────────────────────────────

class ConsoleDisplay;

class UIScreenFader : public UIElement
{
public:
    // speed : progress/초  (기본 4.0 → 0.25 초 전환)
    explicit UIScreenFader(float speed = 4.0f);

    void Render(ConsoleDisplay& display) const override;
    void Update(float deltaTime)               override;
    int  GetZ()  const                         override;

private:
    float m_progress = 0.f;   // 0 = 전체 검정, 1 = 완전 표시
    float m_speed;
};
