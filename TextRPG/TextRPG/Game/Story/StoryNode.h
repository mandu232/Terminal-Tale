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
	std::string sfx;      // 노드 진입 시 1회 재생 효과음 (비어 있으면 없음)

	std::vector<std::string> texts;
	std::vector<StoryChoice> choices;

	std::vector<Effect>    effects;
	std::vector<Condition> require;
};