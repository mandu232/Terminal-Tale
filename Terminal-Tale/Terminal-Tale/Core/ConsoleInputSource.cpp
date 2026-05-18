#include "ConsoleInputSource.h"
#include "Ui/UIManager.h"
#include "InputManager.h"

ConsoleInputSource::ConsoleInputSource(const Settings* settings)
	: settings_(settings)
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

			// 리바인딩 캡처용 raw VK 저장
			input.lastRawVK = static_cast<int>(vk);

			// ── 고정 탐색 키 ──────────────────────────────
			bool handled = true;
			switch ( vk )
			{
			case VK_ESCAPE: input.PushAction(InputAction::Cancel);    break;
			case VK_RETURN: input.PushAction(InputAction::Confirm);   break;
			case VK_LEFT:   input.PushAction(InputAction::MoveLeft);  break;
			case VK_RIGHT:  input.PushAction(InputAction::MoveRight); break;
			case VK_UP:     input.PushAction(InputAction::MoveUp);    break;
			case VK_DOWN:   input.PushAction(InputAction::MoveDown);  break;
			case 0x57:      input.PushAction(InputAction::MoveUp);    break; // W
			case 0x51:      input.PushAction(InputAction::Quit);      break; // Q
			case 0x45:      input.PushAction(InputAction::Confirm);   break; // E
			default:        handled = false;                          break;
			}

			// ── 설정 가능한 단축키 ────────────────────────
			if ( !handled && settings_ )
			{
				const auto& kb = settings_->keyBindings;
				if      ( vk == (WORD)kb.inventoryKey ) input.PushAction(InputAction::OpenInventory);
				else if ( vk == (WORD)kb.waitKey      ) input.PushAction(InputAction::OpenWait);
				else if ( vk == (WORD)kb.sleepKey     ) input.PushAction(InputAction::OpenSleep);
				else if ( vk == (WORD)kb.logKey       ) input.PushAction(InputAction::OpenLog);
				else if ( vk == (WORD)kb.journalKey   ) input.PushAction(InputAction::OpenJournal);
				else if ( vk == (WORD)kb.quickSaveKey ) input.PushAction(InputAction::QuickSave);
				else if ( vk == (WORD)kb.quickLoadKey ) input.PushAction(InputAction::QuickLoad);
			}
		}

		if ( record.EventType == MOUSE_EVENT )
		{
			auto& mouse = record.Event.MouseEvent;
			int x = mouse.dwMousePosition.X + 1;
			int y = mouse.dwMousePosition.Y + 1;

			if ( mouse.dwEventFlags & MOUSE_WHEELED )
			{
				// dwButtonState 상위 워드: 양수 = 위로, 음수 = 아래로
				short delta = static_cast<short>(
					( mouse.dwButtonState >> 16 ) & 0xFFFF );
				if ( delta > 0 )
					input.PushAction(InputAction::ScrollUp);
				else
					input.PushAction(InputAction::ScrollDown);
			}
			else
			{
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
}