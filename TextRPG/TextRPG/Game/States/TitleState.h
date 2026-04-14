#pragma once 

#include "Core/State.h"

class TitleState : public State
{
public:
	TitleState(Context& context);
	void Enter() override;
	void HandleInput(InputManager& input) override;
	void Update() override;
	void Render() override;
};