#pragma once

#include <memory>

class StateMachine;
class InputManager;
class uiManager;
class Context;
class ConsoleBuffer;

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
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<Context> context;
	std::unique_ptr<ConsoleBuffer> buffer;
};