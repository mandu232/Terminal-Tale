#include "ConditionChecker.h"
#include "Core/Context.h"

bool ConditionChecker::Check(
	const std::vector<std::string>& conditions ,
	const Context& ctx)
{
	for ( const auto& r : conditions )
	{
		if ( r.starts_with("flag ") )
		{
			std::string flag = r.substr(5);

			if ( !ctx.flags.contains(flag) )
				return false;
		}
	}

	return true;
}