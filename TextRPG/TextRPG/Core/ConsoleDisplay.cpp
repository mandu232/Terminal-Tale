#include "ConsoleDisplay.h"
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

	// 윈도우 크기를 극단적으로 줄여서 버퍼 수정 시 에러 방지
	SMALL_RECT tmpRect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(hConsole , TRUE , &tmpRect);

	// 버퍼 크기 설정
	COORD bufferSize = { ( SHORT )width, ( SHORT )height };
	SetConsoleScreenBufferSize(hConsole , bufferSize);

	// 실제 윈도우 크기를 버퍼와 1:1로 매칭 (스크롤바 제거 핵심)
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
	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	cfi.cbSize = sizeof(cfi); // 반드시 필요

	if ( !GetCurrentConsoleFontEx(hConsole , FALSE , &cfi) ) return;

	// 1920x1080 모니터, 192x54 버퍼 기준
	cfi.dwFontSize.X = 10;
	cfi.dwFontSize.Y = 20;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName , L"돋움체"); // 고정폭 폰트 사용

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

void ConsoleDisplay::DrawText(int x , int y , const std::wstring& text , short color) {
	int curX = x;
	for ( wchar_t c : text ) {
		if ( curX >= width ) break;

		//현재 위치에 문자 그리기
		Draw(curX , y , c , color);

		if ( c >= 0x1100 ) { // 한글일 경우
			if ( curX + 1 < width ) {
				Draw(curX + 1 , y , L'\0' , color);
			}
			curX += 2;
		}
		else {
			curX += 1;
		}
	}
}

void ConsoleDisplay::Present() {
	COORD origin = { 0, 0 };
	SMALL_RECT writeRegion = { 0, 0, ( SHORT )( width - 1 ), ( SHORT )( height - 1 ) };
	WriteConsoleOutputW(hConsole , screen.data() , { ( SHORT )width, ( SHORT )height } , origin , &writeRegion);
}