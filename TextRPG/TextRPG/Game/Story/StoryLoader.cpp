#include "StoryLoader.h"

#include <fstream>
#include "external/json/json.hpp"

using json = nlohmann::json;

// ── Effect 파싱 ───────────────────────────
static Effect ParseEffect(const json& j)
{
	Effect e;
	std::string type = j.at("type");

	//피로도(체력) 증가
	if ( type == "vitality" ) e.type = EffectType::AddVitality;
	//신뢰도 증가
	else if ( type == "appearance" ) e.type = EffectType::AddAppearance;
	//명성 증가
	else if ( type == "reputation" ) e.type = EffectType::AddRequtation;
	//도덕성 증가
	else if ( type == "karma" ) e.type = EffectType::AddKarma;
	//돈 증가
	else if ( type == "wealth" ) e.type = EffectType::AddWealth;
	//날짜 증가
	else if ( type == "day" )e.type = EffectType::AddDay;
	//시간 증가
	else if ( type == "time" )e.type = EffectType::AddTime;

	//플레그 추가
	else if ( type == "flag_add" ) e.type = EffectType::AddFlag;
	//플레그 제거
	else if ( type == "flag_remove" ) e.type = EffectType::RemoveFlag;

	//아이템 추가
	else if ( type == "give_item" )   e.type = EffectType::GiveItem;
	//아이템 제거
	else if ( type == "remove_item" ) e.type = EffectType::RemoveItem;

	//에러
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


	if ( type == "vitality" ) c.type = ConditionType::Vittality;
	else if(type == "appearance" ) c.type = ConditionType::Appearance;
	else if ( type == "reputation" ) c.type = ConditionType::Reputation;
	else if ( type == "karma" ) c.type = ConditionType::Karma;
	else if ( type == "wealth" ) c.type = ConditionType::Wealth;
	else if ( type == "day" ) c.type = ConditionType::Day;
	else if ( type == "time" ) c.type = ConditionType::Time;

	else if ( type == "flag" ) c.type = ConditionType::Flag;

	else if ( type == "has_item" ) c.type = ConditionType::HasItem;

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