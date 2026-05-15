#include "ItemRegistry.h"
#include "Game/Effect/EffectParser.h"
#include "external/json/json.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

std::unordered_map<std::string, Item> ItemRegistry::s_items;

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
