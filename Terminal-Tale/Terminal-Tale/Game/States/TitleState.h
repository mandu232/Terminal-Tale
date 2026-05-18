#pragma once 

#include "Core/State.h"
#include "Core/EventSubscription.h"

class TitleState : public State
{
private:
	EventSubscription startSub;
	EventSubscription playSound;
	bool receivedStartEvent = false;
public:
	TitleState(Context& context);
	void Enter() override;
	void HandleInput(InputManager& input) override;
	void Update() override;
	void Render(ConsoleDisplay& display) override;
};