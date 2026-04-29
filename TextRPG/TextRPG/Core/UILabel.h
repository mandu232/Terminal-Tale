#pragma once
#include "UIElement.h"
#include <string>
#include <vector>

class UILabel : public UIElement
{
public:
	UILabel(int x , int y , int z, int width, std::string text , short color = 7);

	void Render(ConsoleBuffer& buffer) const override;

	void SetText(std::string newText);

	int GetZ() const override;

private:
	int x;
	int y;
	int maxWidth;
	int zOrder;

	std::vector<std::wstring> wrappedLines;

	void RebuildLayout();

	std::string text;
	short color;
};