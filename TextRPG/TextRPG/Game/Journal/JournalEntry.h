#pragma once
#include <string>

struct JournalEntry
{
    std::string id;           // 중복 추가 방지용 고유 ID
    std::string title;        // 케이스 헤더 (번호 + 대상자 이름/나이)
    std::string outcome;      // 선택한 처리 결과 (선택지 텍스트)
    std::string content;      // 처리 결과 본문 (결과 노드 첫 줄)
    std::string description;  // 사건 내용 전문 (케이스 노드 전체 텍스트)
    int day = 0;              // 처리 날짜
};
