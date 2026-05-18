#include "StoryLoader.h"

#include <fstream>
#include <stdexcept>
#include "external/json/json.hpp"
#include "Game/Effect/EffectParser.h"

using json = nlohmann::json;

// ── Condition 파싱 ─────────────────────────────
static Condition ParseCondition(const json& j)
{
	Condition c;
	std::string type = j.at("type");


	if      ( type == "vitality"   ) c.type = ConditionType::Vitality;
	else if ( type == "reputation" ) c.type = ConditionType::Reputation;
	else if ( type == "wealth"     ) c.type = ConditionType::Wealth;
	else if ( type == "day"        ) c.type = ConditionType::Day;
	else if ( type == "time"       ) c.type = ConditionType::Time;
	else if ( type == "flag"       ) c.type = ConditionType::Flag;
	else if ( type == "has_item"   ) c.type = ConditionType::HasItem;
	else if ( type == "tendency"     ) c.type = ConditionType::Tendency;
	else if ( type == "city_order"   ) c.type = ConditionType::CityOrder;
	else if ( type == "citizen_trust") c.type = ConditionType::CitizenTrust;
	else if ( type == "corruption"   ) c.type = ConditionType::Corruption;

	else throw std::runtime_error("Unknown condition type: " + type);

	if ( j.contains("op") )
	{
		std::string op = j[ "op" ];
		//초과
		if ( op == "gt" ) c.op = ConditionOp::Gt;
		//미만
		else if ( op == "lt" ) c.op = ConditionOp::Lt;
		//동일
		else if ( op == "eq" ) c.op = ConditionOp::Eq;
		//이상
		else if ( op == "gte" ) c.op = ConditionOp::Gte;
		//이하
		else if ( op == "lte" ) c.op = ConditionOp::Lte;
		else throw std::runtime_error("Unknown condition op: " + op);
	}

	if ( j.contains("value") ) c.value = j[ "value" ];
	if ( j.contains("key") ) c.key = j[ "key" ];

	return c;
}

StoryNode StoryLoader::Load(const std::string& path)
{
	std::ifstream file(path);
	if ( !file.is_open() )
		throw std::runtime_error("StoryLoader: Cannot open " + path);

	json j;
	try { file >> j; }
	catch ( const json::parse_error& e )
	{
		throw std::runtime_error("StoryLoader: JSON parse error in " + path + ": " + e.what());
	}

	StoryNode node;

	node.id      = j[ "id" ];
	node.bgImage = j.value("bgImage", "Data/Images/Story/default.png");
	node.bgm     = j.value("bgm"    , "");
	node.sfx     = j.value("sfx"    , "");

	for ( auto& line : j[ "text" ] )
		node.texts.push_back(line);

	if ( j.contains("effects") )
		for ( auto& e : j[ "effects" ] )
			node.effects.push_back(ParseEffect(e));

	if ( j.contains("require") )
		for ( auto& r : j[ "require" ] )
			node.require.push_back(ParseCondition(r));

	for ( auto& choice : j[ "choices" ] )
	{
		StoryChoice c;
		c.text = choice[ "text" ];
		c.nextNode = choice[ "next" ];

		if ( choice.contains("require") )
			for ( auto& r : choice[ "require" ] )
				c.require.push_back(ParseCondition(r));

		if ( choice.contains("effects") )
			for ( auto& e : choice[ "effects" ] )
				c.effects.push_back(ParseEffect(e));

		node.choices.push_back(c);
	}

	return node;
}