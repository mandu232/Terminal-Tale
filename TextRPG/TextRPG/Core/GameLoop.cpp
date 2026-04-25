#include "GameLoop.h"
#include "StateMachine.h"
#include "InputManager.h"
#include "Context.h"
#include "UIManager.h"
#include "ConsoleInputSource.h"
#include "ConsoleBuffer.h"
#include "Game/States/TitleState.h"
#include "Utils/ConsoleUtils.h"

GameLoop::GameLoop()
	: running(true)
{
	SetupConsole(122 , 42);
	HideCursor();

	context = std::make_unique<Context>();
	stateMachine = std::make_unique<StateMachine>(*context);
	inputManager = std::make_unique<InputManager>();

	buffer = std::make_unique<ConsoleBuffer>(120 , 40);

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
	stateMachine->Update();

	if ( context->nextState )
	{
		stateMachine->ChangeState(
			std::move(context->nextState)
		);
	}
}

void GameLoop::Render()
{
	buffer->Clear();
	stateMachine->Render(*buffer);
	buffer->Present();
}