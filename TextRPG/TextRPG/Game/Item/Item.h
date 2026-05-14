#pragma once
#include <string>
#include <vector>
#include "Game/Effect/Effect.h"

struct Item
{
	std::string id;           // 아이템 고유 ID ("health_potion")
	std::string nameKey;      // 현지화 키 ("item.health_potion.name")
	std::string descKey;      // 현지화 키 ("item.health_potion.desc")
	std::vector<Effect> onUseEffects; // 사용 시 발동되는 이펙트
};