#include "UILabel.h"
#include "Core/ConsoleDisplay.h"
#include "Utils/UTF8ToWide.h"
#include "Utils/GetCharWidth.h"

UILabel::UILabel(int x , int y , int z ,
	int width,
	std::string text ,
	short color)
	:
	x(x) ,
	y(y) ,
	maxWidth(width),
	zOrder(z) ,
	color(color)
{
	this->wtext = UTF8ToWide(text);
	RebuildLayout();
}

void UILabel::Render(ConsoleDisplay& display) const
{

	for ( size_t i = 0; i < wrappedLines.size(); ++i ) {
		display.DrawText(x , y + ( int )i , wrappedLines[ i ] , color);
	}
}

int UILabel::GetZ() const
{
	return zOrder;
}

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

		int charWidth = GetCharWidthW(c);

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
}

void UILabel::SetText(std::string newText)
{
	this->wtext = UTF8ToWide(newText);
	RebuildLayout();
}