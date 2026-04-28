#include "Application.h"
#include "Localization.h"
#include "GameLoop.h"
#include "Context.h"

Application::Application()
{
	context = std::make_unique<Context>();

	GContext = context.get();

	context->localization.LoadLocalization("ko");
	gameLoop = std::make_unique<GameLoop>(*context);

}

Application::~Application()
{
}

void Application::Initialize()
{
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
