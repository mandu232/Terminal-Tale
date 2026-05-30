#define NOMINMAX
#include "UIDocumentPanel.h"
#include "Core/ConsoleDisplay.h"
#include "Utils/GetCharWidth.h"

#include <algorithm>

// ─────────────────────────────────────────────
//  파일 내부 상수 · 헬퍼
//
//  레이아웃 (전체 너비 = 58 cols):
//
//  col:  0  1  2  3  4  5 ···  56  57
//        ' '' ''╔''═''═'  ···  '═''╗'   ← 상단 테두리
//        ' '' ''║'' '' '  텍스트  ' ''║'   ← 본문 줄
//        ' '' ''╚''═''═'  ···  '═''╝'   ← 하단 테두리
//
//  PANEL_W  = 58  (전체)
//  LB       = "  │"  (3 cols, 좌 테두리)
//  RB       = "│"    (1 col,  우 테두리)
//  INNER_W  = 54  (PANEL_W - 3 - 1)
//  MARGIN   = "  "   (2 cols, 테두리 안쪽 여백)
//  CONTENT_W= 52  (INNER_W - MARGIN)
//  HDR_DEC  = 50  (헤더/푸터 장식 채움, CONTENT_W - 우측 2칸 여백)
//  TARGET_H = 51  (렌더줄 목표 총합 → 하단 테두리가 화면 row 50 에 위치)
// ─────────────────────────────────────────────
namespace
{
    constexpr int kInnerW   = 54;   // PANEL_W(58) - LB(3) - RB(1)
    constexpr int kContentW = 52;   // kInnerW - 좌측 여백(2)
    constexpr int kHdrDec   = 50;   // kContentW - 우측 여백(2) → 장식 채움 폭
    constexpr int kTargetH  = 51;   // 렌더줄 목표 총합
    constexpr int kFooter   = 4;    // 빈줄 + 장식선 + 텍스트 + 하단 테두리

    // n 개의 wchar_t 반복
    std::wstring rep(wchar_t c, int n)
    {
        return std::wstring(n, c);
    }

    // 한국어(2칸) / 일반(1칸)을 고려한 시각적 너비
    int visualW(const std::wstring& s)
    {
        int w = 0;
        for ( wchar_t c : s )
            w += GetConsoleCharWidth(c);
        return w;
    }

    // text 를 targetW 시각 너비까지 우측 공백 패딩
    std::wstring padR(const std::wstring& text, int targetW)
    {
        int vw  = visualW(text);
        int pad = targetW - vw;
        if ( pad <= 0 ) return text;
        return text + std::wstring(pad, L' ');
    }

    using RL = UIDocumentPanel::RenderLine;

    // 본문 줄: "  |  " + 패딩된 텍스트(kContentW) + "|"   → 총 58 cols
    RL contentLine(const std::wstring& text, short clr)
    {
        return { L"  ║  " + padR(text, kContentW) + L"║", clr };
    }

    // 장식 줄: "  ║  " + fill×kHdrDec + "  ║"            → 총 58 cols
    RL decLine(wchar_t fill, short clr)
    {
        return { L"  ║  " + rep(fill, kHdrDec) + L"  ║", clr };
    }

    // 빈 줄 / 스페이서: "  ║" + kInnerW 공백 + "║"          → 총 58 cols
    RL emptyLine()
    {
        return { L"  ║" + std::wstring(kInnerW, L' ') + L"║", 8 };
    }
}

// 공개 접근을 위해 RenderLine 을 헤더에서 public 으로 선언했으므로
// 헬퍼 함수에서 그대로 사용 가능

// ─────────────────────────────────────────────
//  Rep (static) — 외부 코드에서도 접근 가능하도록 유지
// ─────────────────────────────────────────────
std::wstring UIDocumentPanel::Rep(wchar_t c, int n)
{
    return std::wstring(n, c);
}

// ─────────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────────
UIDocumentPanel::UIDocumentPanel(
    int x, int y, int z,
    std::vector<std::wstring> lines,
    Style style,
    bool  animate)
    : px(x), py(y), pz(z), style(style)
{
    Build(lines);

    if ( animate )
        slideY = -(float)renderLines.size();   // 패널 전체를 화면 위로 숨김
    else
    {
        slideY   = 0.f;
        animDone = true;
    }
}

// ─────────────────────────────────────────────
//  Build — 테두리·헤더·본문·스페이서·푸터 조합
// ─────────────────────────────────────────────
void UIDocumentPanel::Build(const std::vector<std::wstring>& lines)
{
    renderLines.clear();

    const bool isOrder = ( style == Style::Order );

    // ── 상단 여백 3줄 ────────────────────────────
    renderLines.push_back({ L"", 0 });
    renderLines.push_back({ L"", 0 });
    renderLines.push_back({ L"", 0 });

    // ── 상단 테두리  "  ╔" + 54×═ + "╗"  (=58 cols) ─
    renderLines.push_back({ L"  ╔" + rep(L'═', kInnerW) + L"╗", 8 });

    // ── 헤더 ────────────────────────────────────
    if ( !isOrder )
    {
        renderLines.push_back(decLine(L'=', 8));                         // ===
        renderLines.push_back(contentLine(L"       R E C O R D   B U R E A U", 15));
        renderLines.push_back(decLine(L'=', 8));                         // ===
    }
    else
    {
        renderLines.push_back(decLine(L'#', 4));                         // ### (dark red)
        renderLines.push_back(contentLine(L"       R E C O R D   B U R E A U", 15));
        renderLines.push_back(contentLine(L"       명령서 / 집행 문서", 4)); // 명령서 / 집행 문서
        renderLines.push_back(decLine(L'#', 4));                         // ### (dark red)
    }

    // ── 헤더 아래 구분 빈 줄 ─────────────────────
    renderLines.push_back(emptyLine());

    // ── 사진 칸 (Record 전용) ────────────────────
    //  박스 너비 14 cols:  "+" + 12×"-" + "+"
    //  중앙 행  14 cols:   "|" + "  [ 사진 ]  " + "|"  (사·진 각 2칸)
    //  padR(text, 52) 가 나머지 38 cols 를 자동 패딩
    if ( !isOrder )
    {
        renderLines.push_back(contentLine(L"+------------+", 8));
        renderLines.push_back(contentLine(L"|            |", 8));
        renderLines.push_back(contentLine(L"|  [ 사진 ]  |", 8));
        renderLines.push_back(contentLine(L"|            |", 8));
        renderLines.push_back(contentLine(L"+------------+", 8));
        renderLines.push_back(emptyLine());
    }

    // ── 본문 내용 ────────────────────────────────
    for ( const auto& line : lines )
    {
        if ( line.empty() )
            renderLines.push_back(emptyLine());
        else
            renderLines.push_back(contentLine(line, 7));
    }

    // ── 스페이서: kTargetH 를 채우도록 빈 줄 삽입 ─
    int spacers = kTargetH - (int)renderLines.size() - kFooter;
    for ( int i = 0; i < std::max(0, spacers); ++i )
        renderLines.push_back(emptyLine());

    // ── 푸터 ────────────────────────────────────
    renderLines.push_back(emptyLine());                          // 구분 빈 줄
    renderLines.push_back(decLine(L'-', 8));                // ------- 구분선

    if ( !isOrder )
    {
        // "처리 대기 중  -  담당자 결재 요청"
        renderLines.push_back(contentLine(
            L"처리 대기 중"       // 처리 대기 중
            L"  -  "                              // -
            L"담당자 결재 요청", // 담당자 결재 요청
            8));
    }
    else
    {
        // "##  결재 서명 필요  -  즉시 시행  ##"
        renderLines.push_back(contentLine(
            L"##  "                          // ##
            L"결재 서명 필요" // 결재 서명 필요
            L"  -  "                              // -
            L"즉시 시행"               // 즉시 시행
            L"  ##",                         // ##
            12));
    }

    // ── 하단 테두리  "  ╚" + 54×═ + "╝"  (=58 cols) ─
    renderLines.push_back({ L"  ╚" + rep(L'═', kInnerW) + L"╝", 8 });
}

// ─────────────────────────────────────────────
//  GetZ
// ─────────────────────────────────────────────
int UIDocumentPanel::GetZ() const { return pz; }

// ─────────────────────────────────────────────
//  SlideOut — 퇴장 애니메이션 시작 (아래로 슬라이드)
// ─────────────────────────────────────────────
void UIDocumentPanel::SlideOut()
{
    if ( m_dead ) return;          // 이미 죽었으면 무시
    m_slidingOut = true;
    animDone     = false;          // Update 가 다시 동작하도록
    slideY       = 0.f;            // 현재 위치(화면 중간)에서 시작
}

// ─────────────────────────────────────────────
//  Update — 진입(위→0) / 퇴장(0→아래) 슬라이드
// ─────────────────────────────────────────────
void UIDocumentPanel::Update(float deltaTime)
{
    if ( m_slidingOut )
    {
        // 퇴장: slideY 를 양수 방향으로 증가 → 패널이 아래로 이동
        slideY += SLIDE_SPEED * deltaTime;

        if ( slideY >= static_cast<float>( renderLines.size() ) )
            m_dead = true;   // 완전히 화면 밖 → UIManager 가 제거
        return;
    }

    if ( animDone ) return;

    // 진입: slideY 를 0 방향으로 증가 (음수 → 0)
    slideY += SLIDE_SPEED * deltaTime;

    if ( slideY >= 0.f )
    {
        slideY   = 0.f;
        animDone = true;
    }
}

// ─────────────────────────────────────────────
//  Render — slideY 오프셋을 적용해 화면에 출력
//            화면 밖(drawY < 0 또는 >= height) 은 클리핑
// ─────────────────────────────────────────────
void UIDocumentPanel::Render(ConsoleDisplay& display) const
{
    const int offset = static_cast<int>( slideY );   // 음수 또는 0

    for ( int i = 0; i < static_cast<int>( renderLines.size() ); ++i )
    {
        const int drawY = py + offset + i;

        if ( drawY < 0 )                     continue;   // 화면 위 클리핑
        if ( drawY >= display.GetHeight() )  break;      // 화면 아래 클리핑

        const auto& rl = renderLines[ i ];
        if ( !rl.text.empty() )
            display.DrawText(px, drawY, rl.text, rl.color);
    }
}
