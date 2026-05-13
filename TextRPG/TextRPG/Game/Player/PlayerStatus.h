#pragma once
#include <string>
#include <unordered_set>

struct PlayerStats
{

	//플레이어 표기 능력치
	int vitality = 10;       //체력 + 피로도
	int appearance = 100;    //외형 / 신뢰도
	int reputation = 0;      //명성
	int karma = 100;         //도덕성 (기본 100)
	int wealth = 0;          //돈
	int day = 0;             //날짜
	int time = 0;            //시간

	//플레이어 상태 
	//(현재는 안쓰고 있음)
	std::unordered_set<std::string> states;
};