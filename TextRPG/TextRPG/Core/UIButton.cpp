#include "UIButton.h"
#include "ConsoleDisplay.h"
#include "Utils/GetVisualWidth.h"
#include "Utils/UTF8ToWide.h"
#include <cmath>

// ─────────────────────────────────────────────
//  테두리 문자 세트
//  Normal  : ┌─┐ │ │ └─┘
//  Hovered : ╔═╗ ║ ║ ╚═╝
//  Pressed : ▛▀▜ ▌ ▐ ▙▄▟  (굵은 블록)
// ─────────────────────────────────────────────
struct BorderSet
{
	wchar_t tl, tr, bl, br;   // 모서리
	wchar_t h, v;             // 가로선, 세로선
};

static constexpr BorderSet kBorderNormal  = { L'╔', L'╗', L'╚', L'╝', L'═', L'║' };
static constexpr BorderSet kBorderHovered = { L'╔', L'╗', L'╚', L'╝', L'═', L'║' };
static constexpr BorderSet kBorderPressed = { L'▛', L'▜', L'▙', L'▟', L'▀', L'▌' };

// ─────────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────────
UIButton::UIButton(int x , int y , int w , int h ,
	int z ,
	std::string textStr ,
	Callback onClick)
	: x(x) , y(y) , width(w) , height(h) ,
	  zOreder(z) ,
	  onClick(onClick)
{
	this->text = UTF8ToWide(textStr);
}

// ─────────────────────────────────────────────
//  Update — 호버 펄스 애니메이션
//
//  타임라인 (호버 시작 기준)
//  0 ──── GROW_TIME ──── GROW+SHRINK ──── (유지)
//         [확장 구간]    [복귀 구간]       [정지]
// ─────────────────────────────────────────────
void UIButton::Update(float deltaTime)
{
	if (!animRunning) return;

	animTimer += deltaTime;

	// 한 사이클(GROW + SHRINK) 완료 → 정지
	if (animTimer >= GROW_TIME + SHRINK_TIME)
	{
		animTimer   = GROW_TIME + SHRINK_TIME;
		animRunning = false;
	}
}

// ─────────────────────────────────────────────
//  GetAnimOffset
//  구간별 오프셋 반환:
//    [0, GROW_TIME)        → 1 (확장 상태)
//    [GROW_TIME, GROW+SHRINK] → 0 (원래 크기)
// ─────────────────────────────────────────────
int UIButton::GetAnimOffset() const
{
	if (animTimer < GROW_TIME)
		return 1;
	return 0;
}

// ─────────────────────────────────────────────
//  SetHovered
// ─────────────────────────────────────────────
void UIButton::SetHovered(bool value)
{
	State newState = value ? State::Hovered : State::Normal;

	// 호버 진입 시에만 애니메이션 시작
	if (newState == State::Hovered && state != State::Hovered)
	{
		animTimer   = 0.0f;
		animRunning = true;
	}

	// 호버 해제 시 애니메이션 즉시 종료
	if (newState == State::Normal)
	{
		animRunning = false;
		animTimer   = GROW_TIME + SHRINK_TIME; // 오프셋 0으로 고정
	}

	state = newState;
}

// ─────────────────────────────────────────────
//  Contains — 실제 클릭 판정은 원래 크기 기준
// ─────────────────────────────────────────────
bool UIButton::Contains(int mx , int my) const
{
	return mx >= x &&
		mx < x + width &&
		my >= y &&
		my < y + height;
}

void UIButton::Click()
{
	if (onClick) onClick();
}

int UIButton::GetX()      const { return x; }
int UIButton::GetY()      const { return y; }
int UIButton::GetWidth()  const { return width; }
int UIButton::GetHeight() const { return height; }

// ─────────────────────────────────────────────
//  Render
// ─────────────────────────────────────────────
void UIButton::Render(ConsoleDisplay& display) const
{
	short color = GetColor();

	// 애니메이션 오프셋 적용
	int off = GetAnimOffset();
	int rx = x      - off;
	int ry = y      - off;
	int rw = width  + off * 2;
	int rh = height + off * 2;

	// 텍스트를 버튼 중앙에 배치
	int visualWidth = GetVisualWidth(text);
	int textX = rx + ( rw - visualWidth ) / 2;
	int textY = ry + rh / 2;

	if ( borderless ) {
		// 테두리 없이 텍스트만 출력
		display.DrawText(x , y + height / 2 , text , GetColor());
		return;
	}

	// 테두리 세트 선택
	const BorderSet& b =
		(state == State::Pressed) ? kBorderPressed :
		(state == State::Hovered) ? kBorderHovered :
		                            kBorderNormal;

	// ── 1. 배경 + 텍스트 먼저 ────────────────────
	//  한글 등 2셀 와이드 문자는 DrawText 가 다음 셀을 \0 으로 덮음.
	//  텍스트를 먼저 그리고 테두리를 나중에 덮어야 테두리가 보존됨.

	// 배경 (내부 영역)
	for (int i = 1; i < rh - 1; ++i)
		for (int j = 1; j < rw - 1; ++j)
			display.Draw(rx + j , ry + i , L' ' , color);

	display.DrawText(textX , textY , text , color);

	// ── 2. 테두리를 마지막에 덮어씀 ─────────────
	//  텍스트가 테두리 셀을 \0 으로 덮어도 여기서 복원됨.
	for (int i = 0; i < rh; ++i)
	{
		for (int j = 0; j < rw; ++j)
		{
			bool top    = (i == 0);
			bool bottom = (i == rh - 1);
			bool left   = (j == 0);
			bool right  = (j == rw - 1);

			if (!top && !bottom && !left && !right) continue;

			wchar_t c = L' ';
			if      (top    && left)   c = b.tl;
			else if (top    && right)  c = b.tr;
			else if (bottom && left)   c = b.bl;
			else if (bottom && right)  c = b.br;
			else if (top    || bottom) c = b.h;
			else if (left   || right)  c = b.v;

			display.Draw(rx + j , ry + i , c , color);
		}
	}
}

// ─────────────────────────────────────────────
//  GetColor
// ─────────────────────────────────────────────
short UIButton::GetColor() const
{
	switch (state)
	{
	case State::Normal:  return 7;   // 흰색
	case State::Hovered: return 14;  // 노란색
	case State::Pressed: return 10;  // 초록색
	}
	return 7;
}

// ─────────────────────────────────────────────
//  SetText
// ─────────────────────────────────────────────
void UIButton::SetText(std::string newText)
{
	this->text = UTF8ToWide(newText);
}
