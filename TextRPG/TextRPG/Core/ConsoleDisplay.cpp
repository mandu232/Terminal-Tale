#include "ConsoleDisplay.h"
#include <string>
#include "Utils/UTF8ToWide.h"

ConsoleDisplay::ConsoleDisplay(int w , int h)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO ci = { 1, FALSE };
	SetConsoleCursorInfo(hConsole , &ci);

	Resize(w , h);
}

void ConsoleDisplay::Resize(int w , int h)
{
	width = w;
	height = h;

	COORD minSize = { 1, 1 };
	SetConsoleScreenBufferSize(hConsole , minSize);

	SMALL_RECT rect = { 0, 0, ( SHORT )( width - 1 ), ( SHORT )( height - 1 ) };
	SetConsoleWindowInfo(hConsole , TRUE , &rect);

	COORD bufferSize = { ( SHORT )width, ( SHORT )height };
	SetConsoleScreenBufferSize(hConsole , bufferSize);

	screen.assign(width * height , { 0 });
	Clear(0);
}

void ConsoleDisplay::SetFullscreen(bool enable)
{
	HWND hwnd = GetConsoleWindow();

	if ( enable ) 
	{
		savedStyle = GetWindowLong(hwnd , GWL_STYLE);
		GetWindowRect(hwnd, &savedWindowRect);

		HMONITOR monitor = MonitorFromWindow(hwnd , MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(monitor , &mi);

		SetWindowLong(hwnd , GWL_STYLE , savedStyle & ~( WS_CAPTION | WS_THICKFRAME ));
		SetWindowPos(hwnd , HWND_TOP ,
			mi.rcMonitor.left , mi.rcMonitor.top ,
			mi.rcMonitor.right - mi.rcMonitor.left ,
			mi.rcMonitor.bottom - mi.rcMonitor.top ,
			SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		AdaptFontSizeToWindow();
	}
	else
	{
		SetWindowLong(hwnd , GWL_STYLE , savedStyle);
		SetWindowPos(hwnd , nullptr ,
			savedWindowRect.left , savedWindowRect.top ,
			savedWindowRect.right - savedWindowRect.left ,
			savedWindowRect.bottom - savedWindowRect.top ,
			SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		Resize(width , height);
	}
}

void ConsoleDisplay::AdaptFontSizeToWindow() {
	HWND hwnd = GetConsoleWindow();
	RECT clientRect;
	GetClientRect(hwnd , &clientRect);

	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	GetCurrentConsoleFontEx(hConsole , FALSE , &cfi);
	cfi.dwFontSize.X = ( SHORT )( ( clientRect.right - clientRect.left ) / width );
	cfi.dwFontSize.Y = ( SHORT )( ( clientRect.bottom - clientRect.top ) / height );
	SetCurrentConsoleFontEx(hConsole , FALSE , &cfi);
}

void ConsoleDisplay::Clear(short color) {
	for ( auto& cell : screen ) {
		cell.Char.UnicodeChar = L' ';
		cell.Attributes = color;
	}
}

void ConsoleDisplay::Draw(int x , int y , wchar_t c , short color) {
	if ( x < 0 || y < 0 || x >= width || y >= height ) return;
	screen[ y * width + x ] = { {c}, ( WORD )color };
}

void ConsoleDisplay::Present() {
	SMALL_RECT rect = { 0, 0, ( SHORT )( width - 1 ), ( SHORT )( height - 1 ) };
	WriteConsoleOutputW(hConsole , screen.data() , { ( SHORT )width, ( SHORT )height } , { 0, 0 } , &rect);
}