#pragma once
#include <string>

struct LogEntry
{
	std::string text;  // 로그 메시지
	int day = 0;      // 기록 날짜
	int time = 0;      // 기록 시각 (0~23)
};
