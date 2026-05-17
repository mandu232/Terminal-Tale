#pragma once

#include "Effect.h"
#include "external/json/json.hpp"

inline Effect ParseEffect(const nlohmann::json& j)
{
	Effect e;
	std::string type = j.at("type");

	if      ( type == "vitality"    ) e.type = EffectType::AddVitality;
	else if ( type == "reputation"  ) e.type = EffectType::AddReputation;
	else if ( type == "wealth"      ) e.type = EffectType::AddWealth;
	else if ( type == "day"         ) e.type = EffectType::AddDay;
	else if ( type == "time"        ) e.type = EffectType::AddTime;
	else if ( type == "set_time"    ) e.type = EffectType::SetTime;
	else if ( type == "flag_add"    ) e.type = EffectType::AddFlag;
	else if ( type == "flag_remove" ) e.type = EffectType::RemoveFlag;
	else if ( type == "give_item"   ) e.type = EffectType::GiveItem;
	else if ( type == "remove_item" ) e.type = EffectType::RemoveItem;
	else if ( type == "tendency"      ) e.type = EffectType::AddTendency;
	else if ( type == "city_order"    ) e.type = EffectType::AddCityOrder;
	else if ( type == "citizen_trust" ) e.type = EffectType::AddCitizenTrust;
	else if ( type == "corruption"    ) e.type = EffectType::AddCorruption;
	else if ( type == "case_record"   ) e.type = EffectType::CaseRecord;
	else throw std::runtime_error("Unknown effect type: " + type);

	if ( j.contains("value")   ) e.value   = j["value"];
	if ( j.contains("key")     ) e.key     = j["key"];
	if ( j.contains("title")   ) e.title   = j["title"];
	if ( j.contains("outcome") ) e.outcome = j["outcome"];
	if ( j.contains("content") ) e.content = j["content"];

	return e;
}
