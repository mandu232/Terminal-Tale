#include "StoryLoader.h"

#include <fstream>
#include "external/json/json.hpp"

using json = nlohmann::json;

StoryNode StoryLoader::Load(const std::string& path)
{
	std::ifstream file(path);

	json j;
	file >> j;

	StoryNode node;

	node.id = j[ "id" ];

	for ( auto& line : j[ "text" ] )
	{
		node.texts.push_back(line);
	}

	for ( auto& choice : j[ "choices" ] )
	{
		StoryChoice  c;

		c.text = choice[ "text" ];
		c.nextNode = choice[ "next" ];

		node.choices.push_back(c);
	}

	return node;
}