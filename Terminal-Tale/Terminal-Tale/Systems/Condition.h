#pragma once
#include <string>

enum class ConditionType {
	Vitality,
	Reputation,
	Wealth,
	Day,
	Time,
	Flag,
	HasItem,
	Tendency,
	CityOrder,
	CitizenTrust,
	Corruption,
};
enum class ConditionOp { Gt , Lt , Eq , Gte , Lte };

struct Condition
{
	ConditionType type;
	ConditionOp   op = ConditionOp::Eq;
	std::string   key;       // Flag 이름
	int           value = 0; // 값 비교
};