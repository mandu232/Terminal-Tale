#pragma once
#include "Item.h"
#include <string>
#include <unordered_map>

// Data/items.json 에서 아이템 정의를 로드합니다.
// 게임 시작 시 한 번 Load() 를 호출한 뒤,
// Get(id) 로 아이템 정보를 조회하십시오.
class ItemRegistry
{
public:
	static void Load(const std::string& path = "Data/items.json");

	// id 에 해당하는 아이템 정의 반환.
	// 등록되지 않은 id 면 빈 Item(id만 채운) 을 반환합니다.
	static const Item& Get(const std::string& id);

	static bool Has(const std::string& id);

private:
	static std::unordered_map<std::string, Item> s_items;
};
