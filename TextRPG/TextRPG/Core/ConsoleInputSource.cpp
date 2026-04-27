#include "ConsoleInputSource.h"
#include "UIManager.h"
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
			switch ( record.Event.KeyEvent.uChar.AsciiChar )
			{
			case 'w': input.PushAction(InputAction::MoveUp); break;
			case 'q': input.PushAction(InputAction::Quit); break;
			case 'e': input.PushAction(InputAction::Confirm); break;
			}
		}

		if ( record.EventType == MOUSE_EVENT )
		{
			auto& mouse = record.Event.MouseEvent;

			int x = mouse.dwMousePosition.X + 1;
			int y = mouse.dwMousePosition.Y + 1;

			ui.HandleMouseMove(x , y);

			if ( mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED )
			{
				ui.HandleClick(x , y);
			}
		}
	}
}