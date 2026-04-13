#include "Application.h"
#include "GameLoop.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::Initialize()
{
	gameLoop = std::make_unique<GameLoop>();
}

void Application::Shutdown()
{
	gameLoop.reset();
}

void Application::Run()
{
	Initialize();
	gameLoop->Run();
	Shutdown();
}
