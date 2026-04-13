#include "GameLoop.h"
#include "StateMachine.h"
#include "InputManager.h"
#include "Game/States/TitleState.h"

GameLoop::GameLoop()
	: running(true)
{
	stateMachine = std::make_unique<StateMachine>();
	inputManager = std::make_unique<InputManager>();

	stateMachine->ChangeState(
		std::make_unique<TitleState>()
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