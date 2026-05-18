#include "GetCharWidth.h"

int GetConsoleCharWidth(wchar_t c)
{
	if ( c >= 0x1100 ) return 2; // 한글, CJK
	return 1;
}