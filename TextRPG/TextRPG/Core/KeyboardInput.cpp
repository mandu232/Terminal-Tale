#include "KeyboardInput.h"
#include "InputManager.h"
#include <conio.h>

void KeyboardInput::Update(InputManager& input)
{
	if ( _kbhit() )
	{
		if ( !_kbhit() ) return;

		char key = _getch();

		if ( key == 'q' )
			input.PushAction(InputAction::Quit);
		else if (key == 'e' )
			input.PushAction(InputAction::Confirm);
		
	}
}