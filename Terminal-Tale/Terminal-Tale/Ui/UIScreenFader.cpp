#include "UIScreenFader.h"
#include "Core/ConsoleDisplay.h"

// ─────────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────────
UIScreenFader::UIScreenFader(float speed)
    : m_speed(speed)
{
}

// ─────────────────────────────────────────────
//  GetZ — 최상위 z 로 모든 UI 위에 렌더링
// ─────────────────────────────────────────────
int UIScreenFader::GetZ() const { return 9999; }

// ─────────────────────────────────────────────
//  Update — progress 증가 → 1 이상이면 dead
// ─────────────────────────────────────────────
void UIScreenFader::Update(float deltaTime)
{
    if ( m_dead ) return;

    m_progress += m_speed * deltaTime;

    if ( m_progress >= 1.f )
    {
        m_progress = 1.f;
        m_dead     = true;   // UIManager::Update() 가 자동 제거
    }
}

// ─────────────────────────────────────────────
//  Render — 미공개 구간(progress 아래 행)을 검정으로 덮음
//
//  revealedRows = progress * height
//    → 0..revealedRows : 이미 공개된 부분 (렌더링 안 함, 뒤 State 가 그림)
//    → revealedRows..height : 아직 검정 커튼 영역
// ─────────────────────────────────────────────
void UIScreenFader::Render(ConsoleDisplay& display) const
{
    if ( m_dead ) return;

    const int h = display.GetHeight();
    const int w = display.GetWidth();
    const int revealedRows = static_cast<int>( m_progress * h );

    for ( int y = revealedRows; y < h; ++y )
        for ( int x = 0; x < w; ++x )
            display.Draw(x, y, L' ', 0);   // 검정 (color 0)
}
