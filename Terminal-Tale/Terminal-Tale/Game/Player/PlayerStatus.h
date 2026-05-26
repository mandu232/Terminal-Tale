#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>

struct PlayerStats
{
	static constexpr int kMaxFatigue    = 100;
	static constexpr int kMaxMonitoring = 100;

	//플레이어 표기 능력치
	int fatigue      = 0;    //피로도 (0=최상, kMaxFatigue=한계)
	int wealth       = 0;    //돈
	int monitoring   = 0;    //RECORD 감시 등급 (0=정상, kMaxMonitoring=위험)

	// 도시 현황 (세계 수치)
	int cityOrder    = 50;   //도시 질서 (기본 50)
	int citizenTrust = 50;   //시민 신뢰 (기본 50)
	int corruption   = 0;    //오염도 (기록 조작 누적)
	int day = 1;             //날짜
	int time = 8;            //시간

	// 성향 (0 이상, 선택에 따라 누적 증가)
	int empathy    = 0;  // 공감
	int coldness   = 0;  // 냉정
	int justice    = 0;  // 정의
	int compliance = 0;  // 순응
	int suspicion  = 0;  // 의심

	std::unordered_set<std::string> states;
	std::unordered_map<std::string , int> inventory;
};