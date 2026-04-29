#include "UILabel.h"
#include "Core/ConsoleBuffer.h"
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
	text(std::move(text)) ,
	color(color)
{
	RebuildLayout();
}

void UILabel::Render(ConsoleBuffer& buffer) const
{
	for ( size_t i = 0; i < wrappedLines.size(); ++i )
	{
		buffer.DrawText(
			x ,
			y + ( int )i ,
			wrappedLines[i] ,
			7
		);
	}
}

int UILabel::GetZ() const
{
	return zOrder;
}

void UILabel::RebuildLayout()
{
	wrappedLines.clear();

	std::wstring wtext = UTF8ToWide(text);

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
	text = std::move(newText);
	RebuildLayout();
}