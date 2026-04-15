#pragma once

class InputManager;

class InputSource
{
public:
	virtual ~InputSource() = default;
	virtual void Update(InputManager& inputManager) = 0;
};