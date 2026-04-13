#pragma once

class InputManager;

class State
{
public:
	virtual ~State() = default;

	virtual void Enter() {}
	virtual void Exit() {}

	virtual void HandleInput(InputManager& input) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};