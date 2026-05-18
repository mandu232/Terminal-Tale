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
//  FPS와 분리되어 작동(프레임에 따른 출력 속도 차이 최소화)
//
//  ── 속도 단계 (speed 1~5) ──────────────────
//   1 : 느림   ~  6 글자/초
//   2 : 보통   ~ 12 글자/초
//   3 : 기본   ~ 20 글자/초 //가장 기본적인 값
//   4 : 빠름   ~ 35 글자/초
//   5 : 매우빠름~ 60 글자/초
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
    std::function<void()> onChar;       // 글자가 새로 출력될 때 호출 (효과음 등)

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
    int   visibleChars = 0;              // 줄력 중인 글자 수
    int   totalChars   = 0;              // 전체 글자 수
    bool  finished     = false;

    // 효과음 재생 제한 (최대 20회/초)
    static constexpr float kSoundInterval = 0.05f;
    float soundTimer   = kSoundInterval;  // 출력 가능 여부

    // 내부 헬퍼
    void  RebuildLayout();
    float SpeedToCharsPerSec(int speed) const;
};
