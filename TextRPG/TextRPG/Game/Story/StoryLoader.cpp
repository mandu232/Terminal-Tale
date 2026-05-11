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
	node.bgImage = j.value("bgImage" , "Data/Images/Story/default.png");

	for ( auto& line : j[ "text" ] )
	{
		node.texts.push_back(line);
	}

	if ( j.contains("effects") )
	{
		for ( auto& e : j[ "effects" ] )
			node.effects.push_back(e);
	}

	for ( auto& choice : j[ "choices" ] )
	{
		StoryChoice c;

		c.text = choice[ "text" ];
		c.nextNode = choice[ "next" ];

		if ( choice.contains("require") )
		{
			for ( auto& r : choice[ "require" ] )
				c.require.push_back(r);
		}

		node.choices.push_back(c);
	}

	return node;
}