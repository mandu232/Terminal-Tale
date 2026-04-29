#pragma once

#include <memory>
#include <chrono>
#include "State.h"

class StateMachine;
class InputManager;
class uiManager;
class Context;
class ConsoleBuffer;

class GameLoop
{
public:
	GameLoop(Context& ctx);
	~GameLoop();

	void Run();

private:
	void ProcessInput();
	void Update();
	void Render();
	void LimitFPS(std::chrono::high_resolution_clock::time_point frameStart);

	Context& context;

	bool running;
	std::unique_ptr<StateMachine> stateMachine;
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<ConsoleBuffer> buffer;
};