#include "UILabel.h"
#include "Core/ConsoleDisplay.h"
#include "Utils/UTF8ToWide.h"
#include "Utils/GetCharWidth.h"

// ─────────────────────────────────────────────
//  한 줄의 시각적 너비를 구하는 헬퍼
//  (한글 2, 영문/숫자 1 기준)
// ─────────────────────────────────────────────
static int CalcLineWidth(const std::wstring& line)
{
	int w = 0;
	for ( wchar_t c : line )
		w += GetConsoleCharWidth(c);
	return w;
}

// ─────────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────────
UILabel::UILabel(int x , int y , int z , int width , int height ,
	std::string text , short color , TextAlign align , VAlign vAlign)
	:
	x(x) , y(y) , zOrder(z) ,
	maxWidth(width) , maxHeight(height) ,
	color(color) , align(align) , vAlign(vAlign)
{
	this->wtext = UTF8ToWide(text);
	RebuildLayout();
}
// ─────────────────────────────────────────────
//  Render
//  정렬에 따라 각 줄의 시작 x 좌표를 계산
// ─────────────────────────────────────────────
void UILabel::Render(ConsoleDisplay& display) const
{
	// 1. 세로 시작 위치 계산
	int startY = y;
	int totalLines = ( int )wrappedLines.size();

	// 실제 출력할 줄 수는 maxHeight를 넘을 수 없음
	int displayLines = ( totalLines > maxHeight ) ? maxHeight : totalLines;

	if ( vAlign == VAlign::Middle )
	{
		startY = y + ( maxHeight - displayLines ) / 2;
	}
	else if ( vAlign == VAlign::Bottom )
	{
		startY = y + ( maxHeight - displayLines );
	}

	// 2. 텍스트 출력 (maxHeight 범위 내에서만)
	for ( int i = 0; i < displayLines; ++i )
	{
		const auto& line = wrappedLines[ i ];
		int drawX = x;

		// 가로 정렬 계산
		if ( align == TextAlign::Center )
		{
			int lineWidth = CalcLineWidth(line);
			drawX = x + ( maxWidth - lineWidth ) / 2;
		}
		else if ( align == TextAlign::Right )
		{
			int lineWidth = CalcLineWidth(line);
			drawX = x + ( maxWidth - lineWidth );
		}

		display.DrawText(drawX , startY + i , line , color);
	}
}

// ─────────────────────────────────────────────
//  GetZ
// ─────────────────────────────────────────────
int UILabel::GetZ() const
{
	return zOrder;
}

// ─────────────────────────────────────────────
//  RebuildLayout — maxWidth 기준 줄바꿈
// ─────────────────────────────────────────────
void UILabel::RebuildLayout()
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

	// 선택 사항: maxHeight보다 많은 줄은 여기서 미리 잘라낼 수도 있습니다.
	// if (wrappedLines.size() > (size_t)maxHeight) wrappedLines.resize(maxHeight);
}

// ─────────────────────────────────────────────
//  SetText / SetAlign / SetVAlingn
// ─────────────────────────────────────────────
void UILabel::SetText(std::string newText)
{
	this->wtext = UTF8ToWide(newText);
	RebuildLayout();
}

void UILabel::SetAlign(TextAlign newAlign) { align = newAlign; }

void UILabel::SetVAlign(VAlign newVAlign) { vAlign = newVAlign; }