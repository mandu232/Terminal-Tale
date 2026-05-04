#include "SettingState.h"
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include <iostream>
#include <Core/Localization.h>
#include "Core/UIButton.h"
#include "Core/UILabel.h"
#include "Core/UIImage.h"

SettingState::SettingState(Context& context)
	: State(context)
{
}

void SettingState::Enter()
{
	uiManager.Add
	(
		std::make_unique<UILabel>(10 , 10 , 20 , 16 ,
			L("ui.masterVolume"), 7)
	);
	uiManager.Add
	(
		std::make_unique<UILabel>(10 , 14 , 20 , 16 ,
			L("ui.textSpeed") , 7)
	);
	uiManager.Add
	(
		std::make_unique<UILabel>(10 , 18 , 20 , 16 ,
			L("ui.autoSave") , 7)
	);
	uiManager.Add
	(
		std::make_unique<UIButton>(175, 50, 16, 3, 10,
			L("ui.back"),
			[ this ]() {
			context.PopState();
		})
	);
	uiManager.Add
	(
		std::make_unique<UIButton>(155 , 50 , 16 , 3 , 10 ,
			L("ui.save") ,
			[ this ] () {
				context.settingManager.Save("Data/settings.json");
			})
	);

}

void SettingState::HandleInput(InputManager& input)
{
	while ( input.HasAction() )
	{
		auto action = input.PopAction();
	}
}

void SettingState::Update()
{
}

void SettingState::Render(ConsoleDisplay& display)
{
	uiManager.Render(display);

}