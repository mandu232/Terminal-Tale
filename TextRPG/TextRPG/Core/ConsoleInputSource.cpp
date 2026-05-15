#include "ConsoleInputSource.h"
#include "Ui/UIManager.h"
#include "InputManager.h"

ConsoleInputSource::ConsoleInputSource()
{
	hInput = GetStdHandle(STD_INPUT_HANDLE);
}

void ConsoleInputSource::Update(InputManager& input, UIManager& ui)
{
	DWORD eventCount = 0;
	GetNumberOfConsoleInputEvents(hInput , &eventCount);

	if ( eventCount == 0 )
		return;

	std::vector<INPUT_RECORD> records(eventCount);
	DWORD read = 0;

	ReadConsoleInput(hInput , records.data() , eventCount , &read);

	for ( DWORD i = 0; i < read; ++i )
	{
		auto& record = records[ i ];

		if ( record.EventType == KEY_EVENT &&
			record.Event.KeyEvent.bKeyDown )
		{
			WORD vk = record.Event.KeyEvent.wVirtualKeyCode;

			// 가상 키 (VK 코드 기반)
			if ( vk == VK_ESCAPE )
			{
				input.PushAction(InputAction::Cancel);
			}
			else
			{
				// 문자 키 (대소문자 통일)
				char ch = record.Event.KeyEvent.uChar.AsciiChar;
				if ( ch >= 'A' && ch <= 'Z' ) ch += 32; // 대문자 → 소문자

				switch ( ch )
				{
				case 'w': input.PushAction(InputAction::MoveUp);        break;
				case 'q': input.PushAction(InputAction::Quit);          break;
				case 'e': input.PushAction(InputAction::Confirm);       break;
				case 'i': input.PushAction(InputAction::OpenInventory); break;
				}
			}
		}

		if ( record.EventType == MOUSE_EVENT )
		{
			auto& mouse = record.Event.MouseEvent;
			int x = mouse.dwMousePosition.X + 1;
			int y = mouse.dwMousePosition.Y + 1;

			ui.HandleMouseMove(x , y);

			bool isLeftPressedNow = ( mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED );

			if ( isLeftPressedNow && !isLeftPressedBefore )
			{
				ui.HandleClick(x , y);
			}
			isLeftPressedBefore = isLeftPressedNow;
		}
	}
}