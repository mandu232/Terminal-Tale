#pragma once

#include "UIElement.h"
#include <vector>
#include <string>

class UIImage : public UIElement
{
public:
	UIImage(
		int x ,
		int y ,
		int z ,
		const std::string& imagePath ,
		short color = 7
	);

	void Render(ConsoleDisplay& display) const override;
	int GetZ() const override;

private:
	int x;
	int y;
	int zOrder;
	short color;

	std::vector<std::wstring> imageLines;

	void LoadImageW(const std::string& path);
};