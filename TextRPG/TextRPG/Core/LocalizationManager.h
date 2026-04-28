#pragma once
#include <unordered_map>
#include <string>

class LocalizationManager
{
public:
	bool LoadLocalization(const std::string& lang);

	const std::string& Get(const std::string& key) const;

private:
	std::unordered_map<std::string , std::string> texts;
};