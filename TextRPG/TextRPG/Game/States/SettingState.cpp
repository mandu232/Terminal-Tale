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
		std::make_unique<UILabel>(10 , 4 , 20 , 16 ,
			L("ui.setting") , 7)
	);
	uiManager.Add
	(
		std::make_unique<UILabel>(10 , 13 , 20 , 16 ,
			L("ui.fullScreen") , 7)
	);
	uiManager.Add
	(
		std::make_unique<UILabel>(10 , 18 , 20 , 16 ,
			L("ui.masterVolume"), 7)
	);
	uiManager.Add
	(
		std::make_unique<UILabel>(10 , 23 , 20 , 16 ,
			L("ui.textSpeed") , 7)
	);
	uiManager.Add
	(
		std::make_unique<UILabel>(10 , 28 , 20 , 16 ,
			L("ui.autoSave") , 7)
	);
	uiManager.Add
	(
		std::make_unique<UILabel>(10 , 33 , 20 , 16 ,
			L("ui.language") , 7)
	);
	uiManager.Add
	(
		std::make_unique<UILabel>(10 , 38 , 20 , 16 ,
			L("ui.targetFPS") , 7)
	);
	uiManager.Add
	(
		std::make_unique<UILabel>(10 , 43 , 20 , 16 ,
			L("ui.showFPS") , 7)
	);
	uiManager.Add
	(
		std::make_unique<UIButton>(171, 50, 16, 3, 10,
			L("ui.back"),
			[ this ]() {
			context.PopState();
		})
	);
	uiManager.Add
	(
		std::make_unique<UIButton>(150 , 50 , 16 , 3 , 10 ,
			L("ui.save") ,
			[ this ] () {
				context.settingManager.Save("Data/settings.json");
			})
	);
	uiManager.Add
	(
		std::make_unique<UIButton>(129 , 50 , 16 , 3 , 10 ,
			L("ui.reset") ,
			[ this ] () {
				context.settingManager.Load("Data/settings.json");
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