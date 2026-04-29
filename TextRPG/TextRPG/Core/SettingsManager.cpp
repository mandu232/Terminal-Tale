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
	settings.textSpeed = j.value("textSpeed" , 3);
	settings.autoSave = j.value("autoSave" , true);
	settings.language = j.value("language" , "ko");
	settings.targetFPS = j.value("targetFPS" , 30);
	settings.vsync = j.value("vsync" , false);
	settings.showFPS = j.value("showFPS" , false);

	return true;
}



bool SettingsManager::Save(const std::string& path)
{
	json j;

	j[ "masterVolume" ] = settings.masterVolume;
	j[ "textSpeed" ] = settings.textSpeed;
	j[ "autoSave" ] = settings.autoSave;
	j[ "language" ] = settings.language;
	j[ "targetFPS" ] = settings.targetFPS;
	j[ "vsync" ] = settings.vsync;
	j[ "showFPS" ] = settings.showFPS;
		 
	std::ofstream file(path);
	file << j.dump(4);

	return true;
}