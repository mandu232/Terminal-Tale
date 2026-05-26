#pragma once
#include <string>

struct CharacterEntry
{
    std::string id;           // 고유 ID (e.g. "kang_taewon")
    std::string name;         // 이름
    std::string role;         // 직위/역할
    std::string affiliation;  // 소속
    std::string relationship; // 플레이어와의 관계
    std::string description;  // 상세 설명/노트
    int revealedDay = 0;      // 처음 등장한 날짜
};
