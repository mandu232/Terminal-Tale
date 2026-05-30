#include <fstream>
#include "external/json/json.hpp"
#include "SettingsManager.h"

using json = nlohmann::json;



bool SettingsManager::Load(const std::string& path)
{
	std::ifstream file(path);

	if ( !file.is_open() )
		return false;

	json j;
	file >> j;

	settings.masterVolume = j.value("masterVolume" , 100);
	settings.bgmVolume    = j.value("bgmVolume"    , 100);
	settings.sfxVolume    = j.value("sfxVolume"    , 100);
	settings.textSpeed    = j.value("textSpeed"    , 3);
	settings.autoSave = j.value("autoSave" , true);
	settings.language = j.value("language" , "en");
	settings.targetFPS = j.value("targetFPS" , 30);
	settings.vsync = j.value("vsync" , false);
	settings.showFPS = j.value("showFPS" , false);
	settings.fullScreen         = j.value("fullScreen"         , false);
	settings.screenTransition  = j.value("screenTransition"  , true);

	if ( j.contains("keyBindings") )
	{
		const auto& kb = j["keyBindings"];
		settings.keyBindings.inventoryKey = kb.value("inventory",  (int)'I');
		settings.keyBindings.waitKey      = kb.value("wait",       (int)'Z');
		settings.keyBindings.sleepKey     = kb.value("sleep",      (int)'S');
		settings.keyBindings.logKey       = kb.value("log",        (int)'L');
		settings.keyBindings.journalKey   = kb.value("journal",    (int)'J');
		settings.keyBindings.quickSaveKey = kb.value("quickSave",  0x74);
		settings.keyBindings.quickLoadKey = kb.value("quickLoad",  0x78);
	}

	return true;
}



bool SettingsManager::Save(const std::string& path)
{
	json j;

	j[ "masterVolume" ] = settings.masterVolume;
	j[ "bgmVolume"    ] = settings.bgmVolume;
	j[ "sfxVolume"    ] = settings.sfxVolume;
	j[ "textSpeed"    ] = settings.textSpeed;
	j[ "autoSave" ] = settings.autoSave;
	j[ "language" ] = settings.language;
	j[ "targetFPS" ] = settings.targetFPS;
	j[ "vsync" ] = settings.vsync;
	j[ "showFPS" ] = settings.showFPS;
	j[ "fullScreen"        ] = settings.fullScreen;
	j[ "screenTransition" ] = settings.screenTransition;

	{
		json kb;
		kb[ "inventory"  ] = settings.keyBindings.inventoryKey;
		kb[ "wait"       ] = settings.keyBindings.waitKey;
		kb[ "sleep"      ] = settings.keyBindings.sleepKey;
		kb[ "log"        ] = settings.keyBindings.logKey;
		kb[ "journal"    ] = settings.keyBindings.journalKey;
		kb[ "quickSave"  ] = settings.keyBindings.quickSaveKey;
		kb[ "quickLoad"  ] = settings.keyBindings.quickLoadKey;
		j[ "keyBindings" ] = kb;
	}

	std::ofstream file(path);
	file << j.dump(4);

	return true;
}