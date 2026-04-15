#pragma once

#include "InputSource.h"

class KeyboardInput : public InputSource
{
public:
	void Update(InputManager& inputManager) override;
};
