#pragma once

#include <string>

// 스토리에서 사용가능한 이펙트 타입
enum class EffectType
{
	AddHP,
	AddFlag,
	RemoveFlag,
	AddRequtation
};

struct Effect
{
	EffectType type;
	std::string key;
	int value = 0;
};