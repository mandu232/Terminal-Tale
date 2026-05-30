#pragma once
#include "UIElement.h"
#include <string>
#include <vector>

class ConsoleDisplay;

// ─────────────────────────────────────────────
//  UIDocumentPanel
//
//  사건 파일 / 명령서를 좌측 패널에 렌더링합니다.
//  테두리(┌┐│└┘)는 코드로 직접 생성하고,
//  본문 텍스트는 외부에서 주입받습니다.
//
//  ┌──────────────────────────────────────────────────────┐
//  │  ═══════════════ R E C O R D ═══════════════════════ │
//  │                                                      │
//  │  [ 사건 파일 #1042 ]                                 │
//  │  대상 시민 : 한지우 / 34세                           │
//  │  ...                                                 │
//  │                                                      │
//  │  ────────────────────────────────────────────────    │
//  │  처리 대기 중  —  담당자 결재 요청                   │
//  └──────────────────────────────────────────────────────┘
//
//  한국어 문자(2칸)를 포함해도 우측 테두리가 정렬됩니다.
//  등장 시 위에서 슬라이드 내려오는 애니메이션이 재생됩니다.
// ─────────────────────────────────────────────

class UIDocumentPanel : public UIElement
{
public:
    // Record  : ═══ 헤더  /  "처리 대기 중" 푸터
    // Order   : ▓▓▓ 헤더  /  "결재 서명 필요" 푸터
    enum class Style { Record, Order };

    // x, y    : 패널 좌상단 위치
    // z       : z-order
    // lines   : 본문 내용 (테두리 제외, 빈 문자열 = 빈 줄)
    // style   : 헤더/푸터 장식 스타일
    // animate : false 이면 즉시 최종 위치에 표시 (Resume 용)
    UIDocumentPanel(
        int x, int y, int z,
        std::vector<std::wstring> lines,
        Style style   = Style::Record,
        bool  animate = true);

    void Render(ConsoleDisplay& display) const override;
    void Update(float deltaTime)               override;
    int  GetZ()  const                         override;

    // 퇴장: 아래로 슬라이드 후 자동 dead 처리 (UIElement::IsDead() 사용)
    void SlideOut()                            override;

    // 파일 내부 헬퍼 함수가 접근하므로 public 으로 선언
    struct RenderLine
    {
        std::wstring text;
        short        color = 7;
    };

private:
    int   px, py, pz;
    Style style;

    std::vector<RenderLine> renderLines;

    // ── 슬라이드 애니메이션 ───────────────────────
    //  slideY < 0  : 진입 중 (화면 위)
    //  slideY = 0  : 최종 위치
    //  slideY > 0  : 퇴장 중 (화면 아래)
    float slideY      = 0.f;
    bool  animDone    = false;
    bool  m_slidingOut = false;   // 퇴장 애니메이션 중

    static constexpr float SLIDE_SPEED = 90.f;   // 행/초

    // ── 레이아웃 상수 ─────────────────────────────
    //  패널 총 너비(cols)   = PANEL_W  = 58
    //  상·하 내부 폭        = INNER_W  = 54   (PANEL_W - "  │" - "│")
    //  본문 콘텐츠 폭       = CONTENT_W = 52  (INNER_W - 2칸 여백)
    //  헤더·푸터 장식 폭   = HDR_DEC  = 50   (CONTENT_W - 2칸 우측 여백)
    //  목표 전체 줄 수      = TARGET_H = 51   (하단 테두리가 row 50에 위치)
    static constexpr int PANEL_W   = 58;
    static constexpr int INNER_W   = 54;   // PANEL_W - 3(좌) - 1(우)
    static constexpr int CONTENT_W = 52;   // INNER_W - 2(좌 여백)
    static constexpr int HDR_DEC   = 50;   // CONTENT_W - 2(우 여백)
    static constexpr int TARGET_H  = 51;   // 렌더 줄 목표 총합

    void Build(const std::vector<std::wstring>& lines);

    // 헬퍼: n 개의 wchar_t 반복
    static std::wstring Rep(wchar_t c, int n);
};
