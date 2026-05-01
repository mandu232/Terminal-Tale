#include <string>
#include "UTF8ToWide.h"
#include "GetVisualWidth.h"

int GetVisualWidth(const std::wstring& wtext) {
	int width = 0;
	for ( wchar_t c : wtext ) {
		// 유니코드 범위상 한글/특수문자 등(Full-width)은 2칸, 나머지는 1칸
		if ( c >= 0x1100 ) width += 2;
		else width += 1;
	}
	return width;
}