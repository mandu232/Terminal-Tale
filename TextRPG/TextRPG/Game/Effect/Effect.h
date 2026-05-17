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

	CaseRecord ,       //케이스 처리 기록 추가 (처리 문서 열람용)
};

struct Effect
{
	EffectType type = EffectType::AddVitality;
	std::string key;      // 공용 키 (flag 이름, item id, tendency 키, case id 등)
	int value = 0;

	// CaseRecord 전용 필드
	std::string title;    // 케이스 헤더 로컬라이제이션 키 (e.g. "story.case_1053.0")
	std::string outcome;  // 선택한 선택지 로컬라이제이션 키 (e.g. "story.case_1053.choice.0")
	std::string content;  // 결과 본문 로컬라이제이션 키 (e.g. "story.case_1053_a.0")
};