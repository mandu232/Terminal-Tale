#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class ConsoleDisplay
{
private:

	HANDLE hConsole;
	int width;
	int height;
	std::vector<CHAR_INFO> screen;

	// 창 복구를 위한 초기 스타일 저장용
	LONG savedStyle;
	RECT savedWindowRect;

	// 렌더 오프셋 (화면 전환 슬라이드용)
	int m_offsetX = 0;
	int m_offsetY = 0;

	// 화면 전환 스냅샷 — 이전 프레임의 screen 내용을 보존
	std::vector<CHAR_INFO> m_snapshot;

public:

	ConsoleDisplay(int w , int h);

	//해상도 및 창 관리
	void Resize(int w , int h);
	void SetFullscreen(bool enable);
	void AdaptFontSizeToWindow();

	//그리기 기능
	void Clear(short color = 0);
	void Draw(int x , int y , wchar_t c , short color);
	void DrawText(int x , int y , const std::wstring& text , short color);
	void Present();

	// 렌더 오프셋 — 이후 Draw/DrawText 호출에 (dx, dy) 를 더함
	// 화면 전환 슬라이드 애니메이션에 사용; 프레임 끝에 반드시 (0,0) 으로 복원할 것
	void SetOffset(int dx , int dy) { m_offsetX = dx; m_offsetY = dy; }

	// 스냅샷 — 현재 screen 을 복사해 보존; 전환 전 StateMachine 이 호출
	void TakeSnapshot();
	// 스냅샷을 (offsetX, offsetY) 만큼 이동시켜 screen 에 덧씌움
	void BlitSnapshot(int offsetX , int offsetY);

	//게터
	int GetWidth()  const { return width; }
	int GetHeight() const { return height; }
};