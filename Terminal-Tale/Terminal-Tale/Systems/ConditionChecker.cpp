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


	case ConditionType::Vitality:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.vitality > c.value;
		case ConditionOp::Lt:  return ctx.player.vitality < c.value;
		case ConditionOp::Eq:  return ctx.player.vitality == c.value;
		case ConditionOp::Gte: return ctx.player.vitality >= c.value;
		case ConditionOp::Lte: return ctx.player.vitality <= c.value;
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

	case ConditionType::Tendency:
	{
		const auto& p = ctx.player;
		int val = 0;
		if      ( c.key == "empathy"    ) val = p.empathy;
		else if ( c.key == "coldness"   ) val = p.coldness;
		else if ( c.key == "justice"    ) val = p.justice;
		else if ( c.key == "compliance" ) val = p.compliance;
		else if ( c.key == "suspicion"  ) val = p.suspicion;
		switch ( c.op )
		{
		case ConditionOp::Gt:  return val > c.value;
		case ConditionOp::Lt:  return val < c.value;
		case ConditionOp::Eq:  return val == c.value;
		case ConditionOp::Gte: return val >= c.value;
		case ConditionOp::Lte: return val <= c.value;
		}
		return false;
	}

	case ConditionType::CityOrder:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.cityOrder > c.value;
		case ConditionOp::Lt:  return ctx.player.cityOrder < c.value;
		case ConditionOp::Eq:  return ctx.player.cityOrder == c.value;
		case ConditionOp::Gte: return ctx.player.cityOrder >= c.value;
		case ConditionOp::Lte: return ctx.player.cityOrder <= c.value;
		}
		return false;

	case ConditionType::CitizenTrust:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.citizenTrust > c.value;
		case ConditionOp::Lt:  return ctx.player.citizenTrust < c.value;
		case ConditionOp::Eq:  return ctx.player.citizenTrust == c.value;
		case ConditionOp::Gte: return ctx.player.citizenTrust >= c.value;
		case ConditionOp::Lte: return ctx.player.citizenTrust <= c.value;
		}
		return false;

	case ConditionType::Corruption:
		switch ( c.op )
		{
		case ConditionOp::Gt:  return ctx.player.corruption > c.value;
		case ConditionOp::Lt:  return ctx.player.corruption < c.value;
		case ConditionOp::Eq:  return ctx.player.corruption == c.value;
		case ConditionOp::Gte: return ctx.player.corruption >= c.value;
		case ConditionOp::Lte: return ctx.player.corruption <= c.value;
		}
		return false;

	case ConditionType::HasItem:
		auto it = ctx.player.inventory.find(c.key);
		int qty = ( it != ctx.player.inventory.end() ) ? it->second : 0;
		switch ( c.op )
		{
		case ConditionOp::Gt:  return qty > c.value;
		case ConditionOp::Lt:  return qty < c.value;
		case ConditionOp::Eq:  return qty == c.value;
		case ConditionOp::Gte: return qty >= c.value;
		case ConditionOp::Lte: return qty <= c.value;
		}
		return false;
	}

	return false;
}