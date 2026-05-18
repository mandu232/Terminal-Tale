#pragma once 

#include "Core/State.h"
#include "Core/EventSubscription.h"

class GameState : public State
{
private:
	//EventSubscription startSub; 이벤트
public:
	GameState(Context& context);
	void Enter() override;
	void HandleInput(InputManager& input) override;
	void Update() override;
	void Render(ConsoleDisplay& display) override;
};