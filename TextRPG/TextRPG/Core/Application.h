#pragma once

#include <memory>

class GameLoop;
class Context;

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	void Initialize();
	void Shutdown();

	std::unique_ptr<Context> context;
	std::unique_ptr<GameLoop> gameLoop;
};