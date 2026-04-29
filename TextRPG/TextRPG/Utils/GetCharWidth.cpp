#include "GetCharWidth.h"

int GetCharWidthW(wchar_t c)
{
	if ( c >= 0x1100 ) return 2; // 한글, CJK
	return 1;
}