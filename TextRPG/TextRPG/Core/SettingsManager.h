#pragma once
#include <string>

struct Settings
{
	int masterVolume = 100;
	int textSpeed = 3;
	bool autoSave = true;

	std::string language = "ko";
};

class SettingsManager
{
public:
	bool Load(const std::string& path);
	bool Save(const std::string& path);

	Settings settings;
};