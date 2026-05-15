#pragma once
#include <string>

struct JournalEntry
{
    std::string id;       // 중복 추가 방지용 고유 ID
    std::string title;    // 제목
    std::string content;  // 본문
    int day = 0;          // 추가된 날짜
};
