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

		
	case ConditionType::Vittality:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.vitality > c.value;
		case ConditionOp::Lt:  return ctx.player.vitality < c.value;
		case ConditionOp::Eq:  return ctx.player.vitality == c.value;
		case ConditionOp::Gte: return ctx.player.vitality >= c.value;
		case ConditionOp::Lte: return ctx.player.vitality <= c.value;
		}
		return false;

	case ConditionType::Appearance:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.appearance > c.value;
		case ConditionOp::Lt:  return ctx.player.appearance < c.value;
		case ConditionOp::Eq:  return ctx.player.appearance == c.value;
		case ConditionOp::Gte: return ctx.player.appearance >= c.value;
		case ConditionOp::Lte: return ctx.player.appearance <= c.value;
		}
		return false;

	case ConditionType::Reputation:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.reputation > c.value;
		case ConditionOp::Lt:  return ctx.player.reputation < c.value;
		case ConditionOp::Eq:  return ctx.player.reputation == c.value;
		case ConditionOp::Gte: return ctx.player.reputation >= c.value;
		case ConditionOp::Lte: return ctx.player.reputation <= c.value;
		}
		return false;

	case ConditionType::Karma:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.karma > c.value;
		case ConditionOp::Lt:  return ctx.player.karma < c.value;
		case ConditionOp::Eq:  return ctx.player.karma == c.value;
		case ConditionOp::Gte: return ctx.player.karma >= c.value;
		case ConditionOp::Lte: return ctx.player.karma <= c.value;
		}
		return false;

	case ConditionType::Wealth:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.wealth > c.value;
		case ConditionOp::Lt:  return ctx.player.wealth < c.value;
		case ConditionOp::Eq:  return ctx.player.wealth == c.value;
		case ConditionOp::Gte: return ctx.player.wealth >= c.value;
		case ConditionOp::Lte: return ctx.player.wealth <= c.value;
		}
		return false;

	case ConditionType::Day:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.day > c.value;
		case ConditionOp::Lt:  return ctx.player.day < c.value;
		case ConditionOp::Eq:  return ctx.player.day == c.value;
		case ConditionOp::Gte: return ctx.player.day >= c.value;
		case ConditionOp::Lte: return ctx.player.day <= c.value;
		}
		return false;

	case ConditionType::Time:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.time > c.value;
		case ConditionOp::Lt:  return ctx.player.time < c.value;
		case ConditionOp::Eq:  return ctx.player.time == c.value;
		case ConditionOp::Gte: return ctx.player.time >= c.value;
		case ConditionOp::Lte: return ctx.player.time <= c.value;
		}
		return false;
	}


	return false;
}