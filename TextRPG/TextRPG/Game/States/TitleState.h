#pragma once 

#include "Core/State.h"

class TitleState : public State
{
public:
	void Enter() override;
	void HandleInput() override;
	void Update() override;
	void Render() override;
};