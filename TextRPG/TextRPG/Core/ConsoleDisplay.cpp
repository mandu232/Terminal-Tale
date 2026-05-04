#include "ConsoleDisplay.h"
#include "Utils/GetCharwidth.h"
#include "Utils/UTF8ToWide.h"

int GetGlyphWidth(wchar_t c)
{
	// CJK + Fullwidth
	if (
		( c >= 0x1100 && c <= 0x115F ) ||
		( c >= 0x2E80 && c <= 0xA4CF ) ||
		( c >= 0xAC00 && c <= 0xD7A3 ) ||
		( c >= 0xF900 && c <= 0xFAFF ) ||
		( c >= 0xFE10 && c <= 0xFE19 ) ||
		( c >= 0xFF01 && c <= 0xFF60 ) ||
		( c >= 0xFFE0 && c <= 0xFFE6 )
		)
		return 2;

	return 1;
}

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

	// 윈도우 크기를 극단적으로 줄여서 버퍼 수정 시 에러 방지
	SMALL_RECT tmpRect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(hConsole , TRUE , &tmpRect);

	// 버퍼 크기 설정
	COORD bufferSize = { ( SHORT )width, ( SHORT )height };
	SetConsoleScreenBufferSize(hConsole , bufferSize);

	// 실제 윈도우 크기를 버퍼와 1:1로 매칭
	SMALL_RECT finalRect = { 0, 0, ( SHORT )( width - 1 ), ( SHORT )( height - 1 ) };

	// 만약 실패한다면 시스템이 허용하는 최대 크기로 시도
	if ( !SetConsoleWindowInfo(hConsole , TRUE , &finalRect) ) {
		COORD maxWin = GetLargestConsoleWindowSize(hConsole);
		finalRect.Right = ( maxWin.X < width ) ? maxWin.X - 1 : width - 1;
		finalRect.Bottom = ( maxWin.Y < height ) ? maxWin.Y - 1 : height - 1;
		SetConsoleWindowInfo(hConsole , TRUE , &finalRect);
	}

	screen.assign(width * height , { 0 });
	Clear(0);
	SetConsoleCursorPosition(hConsole , { 0, 0 });
}

void ConsoleDisplay::SetFullscreen(bool enable)
{
	HWND hwnd = GetConsoleWindow();
	if ( !enable ) {
		// 창 모드로 복구
		SetWindowLong(hwnd , GWL_STYLE , savedStyle);
		ShowWindow(hwnd , SW_RESTORE);
		Resize(width , height);
		return;
	}

	// --- 전체화면 모드 시작 ---
	savedStyle = GetWindowLong(hwnd , GWL_STYLE);
	GetWindowRect(hwnd , &savedWindowRect);

	// 1. 스타일을 미리 POPUP으로 변경 (테두리 제거 시도)
	SetWindowLong(hwnd , GWL_STYLE , WS_POPUP | WS_VISIBLE);

	// 2. 폰트 강제 설정
	AdaptFontSizeToWindow();

	// 3. 시스템에 ALT + ENTER (전체화면 단축키) 메시지 전송
	// 이게 핵심입니다. API가 못하는 내부 모드 전환을 강제합니다.
	SendMessage(hwnd , WM_SYSCOMMAND , SC_MONITORPOWER , 0); // 화면 깨어남 유도(선택)
	keybd_event(VK_MENU , 0x38 , 0 , 0);                 // ALT Down
	keybd_event(VK_RETURN , 0x1c , 0 , 0);               // ENTER Down
	keybd_event(VK_RETURN , 0x1c , KEYEVENTF_KEYUP , 0); // ENTER Up
	keybd_event(VK_MENU , 0x38 , KEYEVENTF_KEYUP , 0);   // ALT Up

	// 4. 단축키 입력 후 잠시 대기 (터미널이 모드를 바꿀 시간)
	Sleep(50);

	// 5. 바뀐 해상도에 맞춰 다시 한번 위치와 버퍼 조정
	HMONITOR monitor = MonitorFromWindow(hwnd , MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(monitor , &mi);

	SetWindowPos(hwnd , HWND_TOP ,
		mi.rcMonitor.left , mi.rcMonitor.top ,
		mi.rcMonitor.right - mi.rcMonitor.left ,
		mi.rcMonitor.bottom - mi.rcMonitor.top ,
		SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	Resize(width , height);
	Present();
}

void ConsoleDisplay::AdaptFontSizeToWindow() {
	HWND hwnd = GetConsoleWindow();
	HMONITOR monitor = MonitorFromWindow(hwnd , MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(monitor , &mi);

	// 1. 모니터의 가용 픽셀 크기 계산
	int screenWidth = mi.rcMonitor.right - mi.rcMonitor.left;
	int screenHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;

	// 192x59 그리드에 맞춘 폰트 크기 역산
	// 소수점 버림으로 인해 발생하는 여백을 최소화하기 위해 SHORT 캐스팅
	short fontW = ( short )( screenWidth / this->width );
	short fontH = ( short )( screenHeight / this->height );

	// 콘솔 폰트 정보 변경
	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	cfi.cbSize = sizeof(cfi);
	if ( !GetCurrentConsoleFontEx(hConsole , FALSE , &cfi) ) return;

	cfi.dwFontSize.X = fontW; // 계산된 너비 (비율에 따라 가변적)
	cfi.dwFontSize.Y = fontH; // 계산된 높이 (비율에 따라 가변적)
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	// 폰트 지정
	wcscpy_s(cfi.FaceName , L"돋움체");

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

void ConsoleDisplay::DrawText(
	int x ,
	int y ,
	const std::wstring& text ,
	short color)
{
	int curX = x;

	for ( wchar_t c : text )
	{
		if ( curX >= width ) break;

		int w = GetGlyphWidth(c);

		Draw(curX , y , c , color);

		// width 2 문자 처리
		if ( w == 2 && curX + 1 < width )
		{
			screen[ y * width + curX + 1 ].Char.UnicodeChar = L'\0';
			screen[ y * width + curX + 1 ].Attributes = color;
		}

		curX += w;
	}
}

void ConsoleDisplay::Present() {
	COORD origin = { 0, 0 };
	SMALL_RECT writeRegion = { 0, 0, ( SHORT )( width - 1 ), ( SHORT )( height - 1 ) };
	WriteConsoleOutputW(hConsole , screen.data() , { ( SHORT )width, ( SHORT )height } , origin , &writeRegion);
}