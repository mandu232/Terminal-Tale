#include "ConditionChecker.h"
#include "Core/Context.h"

bool ConditionChecker::Check(
	const std::vector<Condition>& conditions , const Context& ctx)
{
	for ( const auto& c : conditions )
		if ( !CheckOne(c , ctx) ) return false; //모두 AND 조건

	return true;
}

bool ConditionChecker::CheckOne(const Condition& c , const Context& ctx)
{
	switch ( c.type )
	{
	case ConditionType::Flag:
		return ctx.flags.count(c.key) > 0;

	case ConditionType::HP:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.hp > c.value;
		case ConditionOp::Lt:  return ctx.player.hp < c.value;
		case ConditionOp::Eq:  return ctx.player.hp == c.value;
		case ConditionOp::Gte: return ctx.player.hp >= c.value;
		case ConditionOp::Lte: return ctx.player.hp <= c.value;
		}
	}
	return false;
}