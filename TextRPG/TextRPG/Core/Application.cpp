#include "Application.h"
#include "Localization.h"
#include "GameLoop.h"
#include "Context.h"
#include "Utils/ConsoleUtils.h"

Application::Application()
{
	SetupConsole();
	context = std::make_unique<Context>();

	context->settings.Load("Data/settings.json");
	context->sound.Init();

	context->sound.SetMasterVolume(
		context->settings.settings.masterVolume
	);

	GContext = context.get();

	context->localization.LoadLocalization(context->settings.settings.language);

	context->ToggleFullscreen(context->settings.settings.fullScreen);

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
