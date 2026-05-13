#include "EffectInterpreter.h"
#include "Core/Context.h"
#include <sstream>

//문자열 파싱 방법(구버전)
void EffectInterpreter::Apply(
	const std::string& effect ,
	Context& ctx)
{
	std::stringstream ss(effect);

	std::string cmd;
	ss >> cmd;
	//피로도 (체력)
	if ( cmd == "vitality" )
	{
		int value;
		ss >> value;
		ctx.player.vitality += value;
	}
	//플레그
	else if ( cmd == "flag" )
	{
		std::string flag;
		ss >> flag;
		ctx.flags.insert(flag);
	}
	//플레그 제거
	else if ( cmd == "flag_remove" )
	{
		std::string flag;
		ss >> flag;
		ctx.flags.erase(flag);
	}
	else if ( cmd == "reputation" )
	{
		int value;
		ss >> value;
		//ctx.player.reputation += value;
	}
}


void EffectInterpreter::Apply(const Effect& effect , Context& ctx)
{
	switch ( effect.type )
	{
	case EffectType::AddVitality:
		ctx.player.vitality += effect.value;
		break;
	case EffectType::AddAppearance:
		ctx.player.appearance += effect.value;
		break;
	case EffectType::AddRequtation:
		ctx.player.reputation += effect.value;
		break;
	case EffectType::AddKarma:
		ctx.player.karma += effect.value;
		break;
	case EffectType:: AddWealth:
		ctx.player.wealth += effect.value;
		break;
	case EffectType::AddDay:
		ctx.player.day += effect.value;
		break;
	case EffectType::AddTime:
		ctx.player.time += effect.value;
		break;


	case EffectType::AddFlag:
		ctx.flags.insert(effect.key);
		break;
	case EffectType::RemoveFlag:
		ctx.flags.erase(effect.key);
		break;
	}
}