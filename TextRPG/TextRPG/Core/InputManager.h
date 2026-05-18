#pragma once 

#include "InputAction.h"
#include "InputSource.h"
#include <memory>
#include <queue>

class UIManager;

class InputManager
{
public:
	void AddSource(std::unique_ptr<InputSource> source);
	void Update(UIManager& ui);

	void PushAction(InputAction action);

	bool HasAction() const;
	InputAction PopAction();
	void Clear();

	int lastRawVK = 0;  // 마지막으로 눌린 raw VK 코드 (키 리바인딩 캡처용)

private:
	std::queue<InputAction> actionQueue;
	std::vector<std::unique_ptr<InputSource>> sources;
};