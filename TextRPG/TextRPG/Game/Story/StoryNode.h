#pragma once

#include <string>
#include <vector>

struct StoryChoice
{
	std::string text;
	std::string nextNode;
};

struct StoryNode
{
	std::string id;

	std::vector < std::string> texts;
	std::vector<StoryChoice> choices;
};