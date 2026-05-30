#define NOMINMAX
#include "StateMachine.h"
#include "InputManager.h"
#include "ConsoleDisplay.h"
#include "Context.h"
#include "State.h"
#include <algorithm>

void StateMachine::ChangeState(std::unique_ptr<State> newState)
{
	while ( !stateStack.empty() )
	{
		stateStack.top()->Exit();
		stateStack.pop();
	}
	PushState(std::move(newState));
}

void StateMachine::HandleInput(InputManager& input)
{
	if (!stateStack.empty())
	{
		stateStack.top()->HandleInput(input);
	}
}

void StateMachine::Update()
{
	UpdateFade();

	if (!stateStack.empty())
	{
		stateStack.top()->Update();
	}
}

void StateMachine::Render(ConsoleDisplay& display)
{
	RenderWithSlide(display);
}

State* StateMachine::GetcurrentState() const
{
	return stateStack.empty() ? nullptr : stateStack.top().get();
}

void StateMachine::PushState(std::unique_ptr<State> newState)
{
	if ( newState )
	{
		stateStack.push(std::move(newState));
		stateStack.top()->Enter();
		StartFade();
	}
}

void StateMachine::PopState()
{
	if ( !stateStack.empty() )
	{
		stateStack.top()->Exit();
		stateStack.pop();
	}
	if ( !stateStack.empty() )
	{
		stateStack.top()->Resume();
		StartFade();
	}
}

// ─────────────────────────────────────────────
//  StartFade — 페이드인 타이머 초기화
// ─────────────────────────────────────────────
void StateMachine::StartFade()
{
	// 설정에서 화면 전환 슬라이드가 꺼져 있으면 즉시 완료 처리
	if ( !context.settingManager.settings.screenTransition )
	{
		m_fadeProgress = 1.f;
		return;
	}

	m_fadeProgress = 0.f;
	m_fadeLastTime = std::chrono::steady_clock::now();

	// 전환 직전 화면을 스냅샷으로 보존
	// (Update 단계이므로 screen 에는 아직 이전 프레임 내용이 남아 있음)
	context.display.TakeSnapshot();
}

// ─────────────────────────────────────────────
//  UpdateFade — 매 Update() 마다 progress 전진
// ─────────────────────────────────────────────
void StateMachine::UpdateFade()
{
	if ( m_fadeProgress >= 1.f ) return;

	auto now = std::chrono::steady_clock::now();
	float dt = std::chrono::duration<float>(now - m_fadeLastTime).count();
	m_fadeLastTime = now;

	if ( dt > 0.f && dt < 1.f )
		m_fadeProgress = std::min(1.f, m_fadeProgress + kFadeSpeed * dt);
}

// ─────────────────────────────────────────────
//  RenderWithSlide — 푸시 슬라이드 전환 렌더
//
//  ease-out quadratic:  eased = t*(2-t)
//  moved = eased * width  (이동한 픽셀 수)
//
//  이전 화면(스냅샷): offset -moved  →  왼쪽으로 밀려 사라짐
//  새 화면           : offset +(width-moved)  →  오른쪽에서 밀려 들어옴
//
//  t=0  : 이전 화면 중앙(offset 0), 새 화면 완전히 오른쪽 밖(offset width)
//  t=0.5: 이전 화면 3/4 왼쪽, 새 화면 1/4 남은 오른쪽
//  t=1  : 이전 화면 완전히 왼쪽 밖, 새 화면 중앙(offset 0)
// ─────────────────────────────────────────────
void StateMachine::RenderWithSlide(ConsoleDisplay& display) const
{
	if ( stateStack.empty() ) return;

	if ( m_fadeProgress < 1.f )
	{
		const float t      = m_fadeProgress;
		const float eased  = t * (2.f - t);                          // ease-out quadratic
		const int   moved  = static_cast<int>( eased * display.GetWidth() );
		const int   w      = display.GetWidth();

		// ① 이전 화면: 스냅샷을 왼쪽으로 moved 만큼 이동
		display.BlitSnapshot(-moved , 0);

		// ② 새 화면: 오른쪽에서 진입 (w - moved 만큼 떨어진 위치)
		display.SetOffset(w - moved , 0);
		stateStack.top()->Render(display);
		display.SetOffset(0 , 0);
	}
	else
	{
		stateStack.top()->Render(display);
	}
}

StateMachine::StateMachine(Context& context) 
	: context(context)
{
}