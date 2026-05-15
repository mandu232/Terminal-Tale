#pragma once

#include <string>

// 스토리에서 사용가능한 이펙트 타입
enum class EffectType
{
	AddVitality,       //체력 증가
	AddReputation,     //명성 증가
	AddWealth,         //돈 증가
	AddDay,            //날짜 증가
	AddTime,           //시간 증가 (상대값)
	SetTime,           //시간 설정 (절댓값)

	AddFlag ,          //플레그 추가
	RemoveFlag ,       //플레그 제거

	GiveItem ,         //아이템 지급
	RemoveItem ,       //아이템 제거

	AddTendency ,      //성향 수치 변화 (key: empathy/coldness/justice/compliance/suspicion)

	AddCityOrder ,     //도시 질서 변화
	AddCitizenTrust ,  //시민 신뢰 변화
	AddCorruption ,    //오염도 변화
};

struct Effect
{
	EffectType type = EffectType::AddVitality;
	std::string key;
	int value = 0;
};