#pragma once

class InputManager;
class UIManager;

class InputSource
{
public:
	virtual ~InputSource() = default;
	virtual void Update(InputManager& inputManager, UIManager& ui) = 0;
};