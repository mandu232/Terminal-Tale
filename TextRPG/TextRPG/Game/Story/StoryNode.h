#pragma once

#include <string>
#include <vector>
#include "Game/Effect/Effect.h"
#include "Systems/Condition.h"   // ← 추가

struct StoryChoice
{
	std::string text;
	std::string nextNode;

	std::vector<Condition> require;
	std::vector<Effect>    effects;
};

struct StoryNode
{
	std::string id;
	std::string bgImage;
	std::string bgm;      // 비어 있으면 현재 BGM 유지

	std::vector<std::string> texts;
	std::vector<StoryChoice> choices;

	std::vector<Effect>    effects;
	std::vector<Condition> require;
};