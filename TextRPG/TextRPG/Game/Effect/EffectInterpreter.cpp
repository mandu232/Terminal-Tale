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
}