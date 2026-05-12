#pragma once
#include "UIElement.h"
#include <string>
#include <vector>
#include <functional>
#include <array>

// ─────────────────────────────────────────────
//  UITypewriter
//
//  UILabel과 동일한 레이아웃(줄바꿈·정렬)을 유지하면서,
//  Update(int fps)가 호출될 때마다 FPS에 무관하게
//  일정한 속도로 글자를 출력합니다.
//
//  ── 속도 단계 (speed 1~5) ──────────────────
//   1 : 느림   ~  6 글자/초   (극적인 연출)
//   2 : 보통   ~ 12 글자/초
//   3 : 기본   ~ 20 글자/초   (권장 기본값)
//   4 : 빠름   ~ 35 글자/초
//   5 : 매우빠름~ 60 글자/초
//
//  ── 프레임 독립 원리 ────────────────────────
//   deltaTime = 1.0f / fps
//   accumulator += deltaTime × charsPerSec
//   → FPS 가 달라져도 초당 출력 글자 수는 동일
//
//  StoryState::RebuildCenter()에서 UILabel →
//  UITypewriter 드롭인 교체가 가능합니다.
// ─────────────────────────────────────────────
class UITypewriter : public UIElement
{
public:
    enum class TextAlign { Left, Center, Right };
    enum class VAlign    { Top,  Middle, Bottom };

    // speed 단계별 초당 글자 수 테이블 (인덱스 0 = speed 1)
    static constexpr std::array<float, 5> kSpeedTable = { 6.f, 12.f, 20.f, 35.f, 60.f };

    // ── 생성자 ───────────────────────────────────
    //  UILabel 과 동일한 인자 순서
    //  speed : 1(느림) ~ 5(빠름),  범위 밖이면 3으로 고정
    UITypewriter(int x, int y, int z,
                 int width, int height,
                 std::string text,
                 short color        = 7,
                 TextAlign align    = TextAlign::Left,
                 VAlign    vAlign   = VAlign::Top,
                 int       speed    = 3);

    // ── UIElement 인터페이스 ─────────────────────
    void Render(ConsoleDisplay& display) const override;

    // fps : 현재 프레임 수 (예: 15, 30, 60)
    // deltaTime = 1/fps 로 내부에서 계산하여 FPS 독립성 보장
    void Update(float fps)      override;

    bool Contains(int x, int y) const override;
    void Click()                      override;   // 클릭 시 즉시 완료
    int  GetZ() const                 override;

    // ── 공개 API ─────────────────────────────────
    void SetText(std::string newText);
    void SetSpeed(int newSpeed);        // 런타임에 속도 변경
    void Skip();                        // 즉시 전체 출력
    bool IsFinished() const;

    std::function<void()> onComplete;   // 출력 완료 시 호출

private:
    // 레이아웃
    int x, y, zOrder;
    int maxWidth, maxHeight;
    short color;
    TextAlign align;
    VAlign    vAlign;

    // 전체 텍스트 (wstring)
    std::wstring wtext;

    // 줄바꿈 후 각 줄 – 전체 텍스트 기준으로 미리 계산 (고정)
    std::vector<std::wstring> wrappedLines;

    // 타이핑 상태
    float charsPerSec  = kSpeedTable[2]; // speed 3 기본값
    float accumulator  = 0.f;            // 소수점 글자 누적
    int   visibleChars = 0;              // 현재 보여줄 글자 수
    int   totalChars   = 0;              // 전체 글자 수
    bool  finished     = false;

    // 내부 헬퍼
    void  RebuildLayout();
    float SpeedToCharsPerSec(int speed) const;
};
