#include "GameLoop.h"
#include "StateMachine.h"
#include "InputManager.h"
#include "Context.h"
#include "ConsoleInputSource.h"
#include "ConsoleBuffer.h"
#include "Game/States/TitleState.h"
#include <thread>

GameLoop::GameLoop(Context& ctx)
	: context(ctx), running(true)
{
	stateMachine = std::make_unique<StateMachine>(context);
	context.stateMachine = stateMachine.get();
	inputManager = std::make_unique<InputManager>();

	buffer = std::make_unique<ConsoleBuffer>(200 , 60);

	inputManager->AddSource(
		std::make_unique<ConsoleInputSource>()
	);

	stateMachine->ChangeState(
		std::make_unique<TitleState>(context)
	);
}

GameLoop::~GameLoop()
{
}

void GameLoop::Run()
{

	using clock = std::chrono::high_resolution_clock;

	while(running)
	{

		auto frameStart = clock::now();

		State* current = stateMachine->GetcurrentState();

		if ( current )
		{
			inputManager->Update(current->GetUIManager());
		}

		ProcessInput();
		Update();
		Render();

		LimitFPS(frameStart);
	}
}

void GameLoop::ProcessInput()
{
	if ( auto* current = stateMachine->GetcurrentState() )
	{
		inputManager->Update(current->GetUIManager());
	}
	stateMachine->HandleInput(*inputManager);
}

void GameLoop::Update()
{
	stateMachine->Update();

	if ( context.nextState )
	{
		stateMachine->ChangeState(
			std::move(context.nextState)
		);
	}
}

void GameLoop::Render()
{
	buffer->Clear();
	stateMachine->Render(*buffer);
	buffer->Present();

}

void GameLoop::LimitFPS(std::chrono::high_resolution_clock::time_point frameStart)
{
	int targetFPS = context.settings.settings.targetFPS;

	if ( targetFPS <= 0 )
		return;

	using namespace std::chrono;

	auto frameTime = high_resolution_clock::now() - frameStart;

	auto targetFrameTime =
		milliseconds(1000 / targetFPS);

	if ( frameTime < targetFrameTime )
	{
		std::this_thread::sleep_for(
			targetFrameTime - frameTime);
	}
}