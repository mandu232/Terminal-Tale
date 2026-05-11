#pragma once
#include <functional>
#include <string>
#include "UIElement.h"

class ConsoleDisplay;

class UIButton : public UIElement
{
public:
	enum class State
	{
		Normal ,
		Hovered ,
		Pressed
	};

	using Callback = std::function<void()>;

	UIButton(int x , int y , int w , int h ,
		int z ,
		std::string text ,
		Callback onClick);

	int GetZ() const override { return zOreder; }

	bool Contains(int mx , int my) const override;
	void Click() override;

	void SetHovered(bool value) override;

	// 매 프레임 호출 — 호버 애니메이션 업데이트
	void Update(float deltaTime) override;

	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;

	void Render(ConsoleDisplay& display) const override;
	short GetColor() const;

	void SetText(std::string newText);

	bool borderless = false;

private:
	int x , y , width , height;
	int zOreder = 0;
	State state = State::Normal;
	std::wstring text;
	Callback onClick;

	// ── 호버 펄스 애니메이션 ──────────────────────
	// 호버가 시작되면 0 → GROW_TIME 동안 +1 확장,
	// 이후 SHRINK_TIME 동안 원래 크기로 복귀, 이후 유지
	static constexpr float GROW_TIME   = 0.12f;  // 커지는 시간 (초)
	static constexpr float SHRINK_TIME = 0.12f;  // 줄어드는 시간 (초)

	float animTimer   = 0.0f;   // 현재 애니메이션 경과 시간
	bool  animRunning = false;  // 애니메이션 진행 중 여부

	// 현재 프레임의 렌더 오프셋 (0 또는 1)
	// 1이면 각 변을 1칸씩 확장해서 그림
	int GetAnimOffset() const;
};
