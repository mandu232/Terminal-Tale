#include "UIImage.h"
#include "Core/ConsoleDisplay.h"
#include "Utils/UTF8ToWide.h"

#include <fstream>

UIImage::UIImage(
	int x ,
	int y ,
	int z ,
	const std::string& imagePath ,
	short color)
	:
	x(x) ,
	y(y) ,
	zOrder(z) ,
	color(color)
{
	LoadImageW(imagePath);
}

void UIImage::LoadImageW(const std::string& path)
{
	imageLines.clear();

	std::ifstream file(path);

	if ( !file.is_open() )
		return;

	std::string line;

	while ( std::getline(file , line) )
	{
		imageLines.push_back(UTF8ToWide(line));
	}
}

int UIImage::GetZ() const
{
	return zOrder;
}

void UIImage::Render(ConsoleDisplay& display) const
{
	for ( size_t i = 0; i < imageLines.size(); ++i )
	{
		display.DrawText(x , y + ( int )i , imageLines[ i ] , 7);
	}
}