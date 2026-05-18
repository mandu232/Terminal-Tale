#include "LocalizationManager.h"
#include "external/json/json.hpp"
#include <fstream>

bool LocalizationManager::LoadLocalization(const std::string& lang)
{
	texts.clear();

	std::string path = "Data/lang/" + lang + ".json";

	std::ifstream file( path );
	nlohmann::json json;
	file >> json;

	for ( auto& [key , value] : json.items() )
		texts[ key ] = value;

	return true;
}

const std::string& LocalizationManager::Get(const std::string& key) const
{
	static std::string missing = "MISSING_TEXT";

	auto it = texts.find(key);
	if ( it == texts.end() )
		return missing;

	return it->second;
}