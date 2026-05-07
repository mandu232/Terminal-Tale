#pragma once

#include "StoryNode.h"

class StoryLoader
{
public:
	static StoryNode Load(const std::string& path);
};