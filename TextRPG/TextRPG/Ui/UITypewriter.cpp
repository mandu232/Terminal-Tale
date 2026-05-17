#include "UITypewriter.h"
#include "Core/ConsoleDisplay.h"
#include "Utils/UTF8ToWide.h"
#include "Utils/GetCharWidth.h"
#include <algorithm>

// ─────────────────────────────────────────────
//  내부 헬퍼: 한 줄의 시각적 너비 계산
// ─────────────────────────────────────────────
static int CalcLineWidth(const std::wstring& line)
{
	int w = 0;
	for ( wchar_t c : line )
		w += GetConsoleCharWidth(c);
	return w;
}

// ─────────────────────────────────────────────
//  SpeedToCharsPerSec — speed 1~5 → 초당 글자 수
// ─────────────────────────────────────────────
float UITypewriter::SpeedToCharsPerSec(int speed) const
{
	int idx = std::clamp(speed , 1 , 5) - 1;   // 0~4
	return kSpeedTable[ idx ];
}

// ─────────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────────
UITypewriter::UITypewriter(int x , int y , int z ,
	int width , int height ,
	std::string text ,
	short color ,
	TextAlign align ,
	VAlign    vAlign ,
	int       speed)
	: x(x) , y(y) , zOrder(z) ,
	maxWidth(width) , maxHeight(height) ,
	color(color) , align(align) , vAlign(vAlign) ,
	charsPerSec(SpeedToCharsPerSec(speed))
{
	wtext = UTF8ToWide(text);
	RebuildLayout();
}

// ─────────────────────────────────────────────
//  Update — fps 를 받아 deltaTime 을 직접 계산
//
//  핵심 원리:
//    deltaTime   = 1.0f / fps          (이번 프레임의 경과 시간)
//    accumulator += deltaTime × charsPerSec  (이번 프레임까지 누적 글자)
//    newChars    = floor(accumulator)   (정수 글자만 실제로 출력)
//    accumulator -= newChars            (소수점 부분은 다음 프레임으로 이월)
//
//  FPS 가 15 든 30 든 60 이든 "초당 charsPerSec 글자"라는
//  결과는 동일합니다.
// ─────────────────────────────────────────────
void UITypewriter::Update(float deltaTime)
{
	if ( finished ) return;
	if ( deltaTime <= 0.f ) return;

	// UIManager 가 이미 1/fps 로 변환한 deltaTime 을 그대로 사용
	accumulator += deltaTime * charsPerSec;

	const int newChars = static_cast< int >( accumulator );  // 정수 부분만 반영

	if ( newChars > 0 )
	{
		accumulator -= static_cast< float >( newChars );    // 소수점 이월
		visibleChars += newChars;

		// 효과음: kSoundInterval(50ms)마다 최대 1회 재생
		soundTimer += deltaTime;
		if ( onChar && soundTimer >= kSoundInterval )
		{
			soundTimer = 0.f;
			onChar();
		}

		if ( visibleChars >= totalChars )
		{
			visibleChars = totalChars;
			finished = true;
			if ( onComplete ) onComplete();
		}
	}
}

// ─────────────────────────────────────────────
//  Render — visibleChars 만큼만 출력
//
//  wrappedLines 를 순서대로 훑으며
//  visibleChars 예산이 남아 있는 만큼만 그립니다.
// ─────────────────────────────────────────────
void UITypewriter::Render(ConsoleDisplay& display) const
{
	// 1. 세로 시작 위치
	int totalLines = static_cast< int >( wrappedLines.size() );
	int displayLines = ( totalLines > maxHeight ) ? maxHeight : totalLines;

	int startY = y;
	if ( vAlign == VAlign::Middle )
		startY = y + ( maxHeight - displayLines ) / 2;
	else if ( vAlign == VAlign::Bottom )
		startY = y + ( maxHeight - displayLines );

	// 2. 글자 예산
	int remaining = visibleChars;

	for ( int i = 0; i < displayLines && remaining > 0; ++i )
	{
		const std::wstring& fullLine = wrappedLines[ i ];

		// 이 줄에서 실제로 보여줄 글자 수 결정
		int show = static_cast< int >( fullLine.size() );
		if ( show > remaining )
			show = remaining;

		std::wstring visiblePart = fullLine.substr(0 , show);
		remaining -= show;

		// 가로 정렬 – 전체 줄 너비(fullLine) 기준으로 맞춰야
		// 글자가 늘어날 때 텍스트가 흔들리지 않습니다.
		int drawX = x;
		if ( align == TextAlign::Center )
		{
			int fullWidth = CalcLineWidth(fullLine);
			drawX = x + ( maxWidth - fullWidth ) / 2;
		}
		else if ( align == TextAlign::Right )
		{
			int fullWidth = CalcLineWidth(fullLine);
			drawX = x + ( maxWidth - fullWidth );
		}

		display.DrawText(drawX , startY + i , visiblePart , color);
	}
}

// ─────────────────────────────────────────────
//  Contains / Click (클릭 시 즉시 전체 출력)
// ─────────────────────────────────────────────
bool UITypewriter::Contains(int cx , int cy) const
{
	return ( cx >= x && cx < x + maxWidth &&
		cy >= y && cy < y + maxHeight );
}

void UITypewriter::Click()
{
	if ( !finished )
		Skip();
}

// ─────────────────────────────────────────────
//  Skip — 즉시 완료
// ─────────────────────────────────────────────
void UITypewriter::Skip()
{
	visibleChars = totalChars;
	finished = true;
	if ( onComplete ) onComplete();
}

bool UITypewriter::IsFinished() const { return finished; }

int  UITypewriter::GetZ() const { return zOrder; }

// ─────────────────────────────────────────────
//  SetSpeed — 런타임에 speed 단계 변경 (1~5)
// ─────────────────────────────────────────────
void UITypewriter::SetSpeed(int newSpeed)
{
	charsPerSec = SpeedToCharsPerSec(newSpeed);
	accumulator = 0.f;   // 이월 값 초기화 (속도 전환 시 튀는 현상 방지)
}

// ─────────────────────────────────────────────
//  SetText — 텍스트 교체 후 처음부터 다시 시작
// ─────────────────────────────────────────────
void UITypewriter::SetText(std::string newText)
{
	wtext = UTF8ToWide(newText);
	accumulator = 0.f;
	visibleChars = 0;
	finished = false;
	RebuildLayout();
}

// ─────────────────────────────────────────────
//  RebuildLayout — UILabel과 동일한 줄바꿈 로직
// ─────────────────────────────────────────────
void UITypewriter::RebuildLayout()
{
	wrappedLines.clear();
	std::wstring currentLine;
	int currentWidth = 0;

	for ( wchar_t c : wtext )
	{
		if ( c == L'\n' )
		{
			wrappedLines.push_back(currentLine);
			currentLine.clear();
			currentWidth = 0;
			continue;
		}

		int charWidth = GetConsoleCharWidth(c);

		if ( currentWidth + charWidth > maxWidth )
		{
			wrappedLines.push_back(currentLine);
			currentLine.clear();
			currentWidth = 0;
		}

		currentLine += c;
		currentWidth += charWidth;
	}

	if ( !currentLine.empty() )
		wrappedLines.push_back(currentLine);

	// 전체 글자 수 집계
	totalChars = 0;
	for ( const auto& line : wrappedLines )
		totalChars += static_cast< int >( line.size() );
}