#pragma once

#include <string>

// 스토리에서 사용가능한 이펙트 타입
enum class EffectType
{
	AddVitality,       //피로도(체력) 증가
	AddAppearance,     //신뢰도 증가
	AddReputation,     //명성 증가
	AddKarma,          //카르마(선악) 증가
	AddWealth,         //돈 증가
	AddDay,            //날짜 증가
	AddTime,           //시간 증가

	AddFlag ,          //플레그 추가
	RemoveFlag ,       //플레그 제거

	GiveItem ,         //아이템 지급
	RemoveItem ,       //아이템 제거
};

struct Effect
{
	EffectType type = EffectType::AddVitality;
	std::string key;
	int value = 0;
};