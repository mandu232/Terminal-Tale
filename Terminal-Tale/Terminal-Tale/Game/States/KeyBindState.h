#pragma once
#include "Core/State.h"
#include "Ui/UILabel.h"
#include "Ui/UIButton.h"
#include <vector>
#include <string>

class KeyBindState : public State
{
public:
	explicit KeyBindState(Context& context);

	void Enter()                           override;
	void Exit()                            override {}
	void HandleInput(InputManager& input)  override;
	void Update()                          override {}
	void Render(ConsoleDisplay& display)   override;

private:
	static std::string VkToDisplay(int vk);
	void CancelRebind();

	struct BindRow
	{
		std::string labelKey;
		int*        value      = nullptr;
		UILabel*    keyLabel   = nullptr;
		UIButton*   changeBtn  = nullptr;
	};

	std::vector<BindRow> rows_;
	int rebindIndex_ = -1;
};
