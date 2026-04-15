#include "GameLoop.h"
#include "StateMachine.h"
#include "InputManager.h"
#include "Context.h"
#include "UIManager.h"
#include "ConsoleInputSource.h"
#include "Game/States/TitleState.h"
#include "Utils/ConsoleUtils.h"

GameLoop::GameLoop()
	: running(true)
{
	HideCursor();

	context = std::make_unique<Context>();
	stateMachine = std::make_unique<StateMachine>(*context);
	inputManager = std::make_unique<InputManager>();

	inputManager->AddSource(
		std::make_unique<ConsoleInputSource>(context->uiManager)
	);

	stateMachine->ChangeState(
		std::make_unique<TitleState>(*context)
	);
}

GameLoop::~GameLoop()
{
}

void GameLoop::Run()
{
	while(running)
	{
		inputManager->Update();

		ProcessInput();
		Update();
		Render();
	}
}

void GameLoop::ProcessInput()
{
	stateMachine->HandleInput(*inputManager);
}

void GameLoop::Update()
{
}

void GameLoop::Render()
{
	stateMachine->Render();
}