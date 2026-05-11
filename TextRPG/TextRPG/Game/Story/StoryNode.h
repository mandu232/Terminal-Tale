#pragma once

#include <string>
#include <vector>

struct StoryChoice
{
	std::string text;
	std::string nextNode;

	// 선택 조건
	std::vector<std::string> require;
};

struct StoryNode
{
	std::string id;
	std::string bgImage;

	std::vector < std::string> texts;
	std::vector<StoryChoice> choices;

	//노드 진입 시 실행되는 효과
	std::vector< std::string> effects;

	//노드 표시 조건
	std::vector<std::string> require;
};