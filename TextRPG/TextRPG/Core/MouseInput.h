#pragma once

#include "InputSource.h"

class MouseInput : public InputSource
{
public:
	void Update(InputManager& input) override;
};

