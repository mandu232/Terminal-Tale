#pragma once

#include "Effect.h"
#include "external/json/json.hpp"

inline Effect ParseEffect(const nlohmann::json& j)
{
	Effect e;
	std::string type = j.at("type");

	if      ( type == "vitality"    ) e.type = EffectType::AddVitality;
	else if ( type == "appearance"  ) e.type = EffectType::AddAppearance;
	else if ( type == "reputation"  ) e.type = EffectType::AddReputation;
	else if ( type == "karma"       ) e.type = EffectType::AddKarma;
	else if ( type == "wealth"      ) e.type = EffectType::AddWealth;
	else if ( type == "day"         ) e.type = EffectType::AddDay;
	else if ( type == "time"        ) e.type = EffectType::AddTime;
	else if ( type == "flag_add"    ) e.type = EffectType::AddFlag;
	else if ( type == "flag_remove" ) e.type = EffectType::RemoveFlag;
	else if ( type == "give_item"   ) e.type = EffectType::GiveItem;
	else if ( type == "remove_item" ) e.type = EffectType::RemoveItem;
	else throw std::runtime_error("Unknown effect type: " + type);

	if ( j.contains("value") ) e.value = j["value"];
	if ( j.contains("key")   ) e.key   = j["key"];

	return e;
}
