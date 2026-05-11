#include "EffectInterpreter.h"
#include "Core/Context.h"
#include <sstream>

void EffectInterpreter::Apply(
	const std::string& effect ,
	Context& ctx)
{
	std::stringstream ss(effect);

	std::string cmd;
	ss >> cmd;

	if ( cmd == "hp" )
	{
		int value;
		ss >> value;
		ctx.player.hp += value;
	}
	else if ( cmd == "flag" )
	{
		std::string flag;
		ss >> flag;
		ctx.flags.insert(flag);
	}
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
	case EffectType::AddHP:
		ctx.player.hp += effect.value;
		break;
	case EffectType::AddFlag:
		ctx.flags.insert(effect.key);
		break;
	case EffectType::RemoveFlag:
		ctx.flags.erase(effect.key);
		break;
	case EffectType::AddRequtation:       // 오타 그대로 enum에 맞춤
		//ctx.player.reputation += effect.value;
		break;
	}
}