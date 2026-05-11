#pragma once
#include <string>

enum class ConditionType { HP , Flag };
enum class ConditionOp { Gt , Lt , Eq , Gte , Lte };

struct Condition
{
	ConditionType type;
	ConditionOp   op = ConditionOp::Eq;
	std::string   key;       // Flag 이름
	int           value = 0; // HP 비교값
};