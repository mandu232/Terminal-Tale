#include "ItemRegistry.h"
#include "Game/Story/StoryLoader.h"   // ParseEffect 재사용을 위해 포함
#include "external/json/json.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

std::unordered_map<std::string, Item> ItemRegistry::s_items;

// ── Effect 파싱 (StoryLoader 와 동일한 로직) ─────────────────────────────────
static Effect ParseEffect(const json& j)
{
	Effect e;
	std::string type = j.at("type");

	if      ( type == "vitality"     ) e.type = EffectType::AddVitality;
	else if ( type == "appearance"   ) e.type = EffectType::AddAppearance;
	else if ( type == "reputation"   ) e.type = EffectType::AddRequtation;
	else if ( type == "karma"        ) e.type = EffectType::AddKarma;
	else if ( type == "wealth"       ) e.type = EffectType::AddWealth;
	else if ( type == "day"          ) e.type = EffectType::AddDay;
	else if ( type == "time"         ) e.type = EffectType::AddTime;
	else if ( type == "flag_add"     ) e.type = EffectType::AddFlag;
	else if ( type == "flag_remove"  ) e.type = EffectType::RemoveFlag;
	else if ( type == "give_item"    ) e.type = EffectType::GiveItem;
	else if ( type == "remove_item"  ) e.type = EffectType::RemoveItem;
	else throw std::runtime_error("ItemRegistry: Unknown effect type: " + type);

	if ( j.contains("value") ) e.value = j["value"];
	if ( j.contains("key")   ) e.key   = j["key"];

	return e;
}

// ── Load ────────────────────────────────────────────────────────────────────
void ItemRegistry::Load(const std::string& path)
{
	std::ifstream file(path);
	if ( !file.is_open() )
		throw std::runtime_error("ItemRegistry: Cannot open " + path);

	json j;
	file >> j;

	s_items.clear();

	for ( const auto& obj : j )
	{
		Item item;
		item.id      = obj.at("id");
		item.nameKey = obj.at("name");
		item.descKey = obj.at("desc");
		item.usable  = obj.value("usable", true);

		if ( obj.contains("effects") )
			for ( const auto& e : obj["effects"] )
				item.onUseEffects.push_back(ParseEffect(e));

		s_items[item.id] = std::move(item);
	}
}

// ── Get ─────────────────────────────────────────────────────────────────────
const Item& ItemRegistry::Get(const std::string& id)
{
	auto it = s_items.find(id);
	if ( it != s_items.end() )
		return it->second;

	// 등록되지 않은 아이템은 임시 항목을 반환
	static thread_local Item fallback;
	fallback     = Item{};
	fallback.id  = id;
	return fallback;
}

bool ItemRegistry::Has(const std::string& id)
{
	return s_items.count(id) > 0;
}
