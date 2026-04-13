#include "GameLoop.h"
#include "StateMachine.h"
#include "Game/States/TitleState.h"

GameLoop::GameLoop()
	: running(true)
{
	stateMachine = std::make_unique<StateMachine>();

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
		ProcessInput();
		Update();
		Render();
	}
}

void GameLoop::ProcessInput()
{
	stateMachine->HandleInput();
}

void GameLoop::Update()
{
}

void GameLoop::Render()
{
	stateMachine->Render();
}