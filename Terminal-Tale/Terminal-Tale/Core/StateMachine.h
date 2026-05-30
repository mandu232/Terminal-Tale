#pragma once

#include <memory>
#include <stack>
#include <chrono>

class State;
class InputManager;
class Context;
class ConsoleDisplay;

class StateMachine
{
public:
	void ChangeState(std::unique_ptr<State> newState);
	void PushState(std::unique_ptr<State> newState);
	void PopState();

	void HandleInput(InputManager& input);
	void Update();
	void Render(ConsoleDisplay& display);

	StateMachine(Context& context);

	State* GetcurrentState() const;

private:
	std::stack<std::unique_ptr<State>> stateStack;
	Context& context;

	// ── 화면 전환 페이드인 오버레이 ─────────────────
	//  모든 State 전환(Push / Pop)에서 자동으로 재생됨.
	//  StateMachine 이 직접 타이밍을 관리하므로
	//  개별 State 가 uiManager.Update() 를 호출하지 않아도 동작함.
	float m_fadeProgress = 1.f;    // 0=전체 검정, 1=완전 표시(오버레이 없음)
	static constexpr float kFadeSpeed = 4.0f;   // progress/초 → 0.25 초 전환
	std::chrono::steady_clock::time_point m_fadeLastTime;

	void StartFade();                             // PushState / PopState 에서 호출
	void UpdateFade();                            // Update() 내부에서 호출
	void RenderWithSlide(ConsoleDisplay& display) const;  // Render() 내부에서 호출
};