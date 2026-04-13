#pragma once

#include <memory>

class StateMachine;

class GameLoop
{
public:
	GameLoop();
	~GameLoop();

	void Run();

private:
	void ProcessInput();
	void Update();
	void Render();

private:
	bool running;
	std::unique_ptr<StateMachine> stateMachine;
};