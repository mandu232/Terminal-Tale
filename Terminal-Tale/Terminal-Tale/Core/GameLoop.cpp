#include "GameLoop.h"
#include "StateMachine.h"
#include "InputManager.h"
#include "Context.h"
#include "ConsoleInputSource.h"
#include "Game/States/TitleState.h"
#include <thread>

GameLoop::GameLoop(Context& ctx)
	: context(ctx), running(true)
{
	stateMachine = std::make_unique<StateMachine>(context);
	context.stateMachine = stateMachine.get();
	inputManager = std::make_unique<InputManager>();

	inputManager->AddSource(
		std::make_unique<ConsoleInputSource>(&context.settingManager.settings)
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

	// 입력 처리 중 쌓인 지연 상태 전환을 여기서 일괄 적용한다.
	// (ProcessInput 도중 즉시 전환하면 UIManager use-after-free가 발생함)
	bool stateChanged = !context.pendingStateOps.empty();
	for ( auto& op : context.pendingStateOps )
	{
		switch ( op.type )
		{
		case Context::PendingStateOp::Type::Push:
			stateMachine->PushState(std::move(op.state));
			break;
		case Context::PendingStateOp::Type::Pop:
			stateMachine->PopState();
			break;
		case Context::PendingStateOp::Type::Change:
			stateMachine->ChangeState(std::move(op.state));
			break;
		}
	}
	context.pendingStateOps.clear();

	if ( context.nextState )
	{
		stateMachine->ChangeState(std::move(context.nextState));
		stateChanged = true;
	}

	// 상태가 바뀌었으면 이전 상태용 stale 입력을 버린다.
	if ( stateChanged )
		inputManager->Clear();
}

void GameLoop::Render()
{
	context.display.Clear();
	stateMachine->Render(context.display);
	context.display.Present();

}

void GameLoop::LimitFPS(std::chrono::high_resolution_clock::time_point frameStart)
{
	int targetFPS = context.settingManager.settings.targetFPS;

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