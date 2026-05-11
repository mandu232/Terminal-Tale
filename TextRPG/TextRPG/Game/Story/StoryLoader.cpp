#include "StoryLoader.h"

#include <fstream>
#include "external/json/json.hpp"

using json = nlohmann::json;

// ── Effect 파싱 ───────────────────────────
static Effect ParseEffect(const json& j)
{
	Effect e;
	std::string type = j.at("type");

	if ( type == "hp" ) e.type = EffectType::AddHP;
	else if ( type == "flag_add" ) e.type = EffectType::AddFlag;
	else if ( type == "flag_remove" ) e.type = EffectType::RemoveFlag;
	else throw std::runtime_error("Unknown effect type: " + type);

	if ( j.contains("value") ) e.value = j[ "value" ];
	if ( j.contains("key") ) e.key = j[ "key" ];

	return e;
}

// ── Condition 파싱 ─────────────────────────────
static Condition ParseCondition(const json& j)
{
	Condition c;
	std::string type = j.at("type");

	if ( type == "hp" ) c.type = ConditionType::HP;
	else if ( type == "flag" ) c.type = ConditionType::Flag;
	else throw std::runtime_error("Unknown condition type: " + type);

	if ( j.contains("op") )
	{
		std::string op = j[ "op" ];
		if ( op == "gt" ) c.op = ConditionOp::Gt;
		else if ( op == "lt" ) c.op = ConditionOp::Lt;
		else if ( op == "eq" ) c.op = ConditionOp::Eq;
		else if ( op == "gte" ) c.op = ConditionOp::Gte;
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
	json j;
	file >> j;

	StoryNode node;

	node.id = j[ "id" ];
	node.bgImage = j.value("bgImage" , "Data/Images/Story/default.png");

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