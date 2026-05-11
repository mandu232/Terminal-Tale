#pragma once
#include <string>
#include <unordered_set>

struct PlayerStats
{
	int hp = 10;
	int maxHp = 10;

	int reputationVillage = 0;
	int reputationForest = 0;

	std::unordered_set<std::string> states;
};