#include "Application.h"
#include "Localization.h"
#include "GameLoop.h"
#include "Context.h"
#include "Utils/ConsoleUtils.h"

Application::Application()
{
	SetupConsole();
	context = std::make_unique<Context>();

	context->settingManager.Load("Data/settings.json");
	context->sound.Init();

	auto& s = context->settingManager.settings;
	context->sound.SetMasterVolume(s.masterVolume / 100.0f);
	context->sound.SetBGMVolume   (s.bgmVolume    / 100.0f);
	context->sound.SetSFXVolume   (s.sfxVolume    / 100.0f);

	GContext = context.get();

	context->localization.LoadLocalization(context->settingManager.settings.language);

	context->ToggleFullscreen(context->settingManager.settings.fullScreen);

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
