#pragma once
#include "UIElement.h"
#include <string> 
#include <vector>

class UILabel : public UIElement
{
public:
	enum class TextAlign
	{
		Left ,
		Center ,
		Right
	};

	// 세로 정렬 추가
	enum class VAlign
	{
		Top ,
		Middle ,
		Bottom
	};

	// 생성자에 height와 vAlign 매개변수 추가
	UILabel(int x , int y , int z , int width , int height ,
		std::string text ,
		short color = 7 ,
		TextAlign align = TextAlign::Left ,
		VAlign vAlign = VAlign::Top);

	void Render(ConsoleDisplay& display) const override;

	void SetText(std::string newText);
	void SetAlign(TextAlign newAlign);
	void SetVAlign(VAlign newVAlign); // 세로 정렬 설정 함수

	int GetZ() const override;

private:
	int x;
	int y;
	int maxWidth;
	int maxHeight; // 높이 값 추가
	int zOrder;
	short color;
	TextAlign align;
	VAlign vAlign; // 세로 정렬 방식 저장

	std::wstring wtext;
	std::vector<std::wstring> wrappedLines;

	void RebuildLayout();
};
