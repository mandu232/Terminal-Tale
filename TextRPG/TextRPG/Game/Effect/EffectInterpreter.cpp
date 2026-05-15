#include "EffectInterpreter.h"
#include "Core/Context.h"

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
	case EffectType::AddReputation:
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

	case EffectType::GiveItem:
		ctx.player.inventory[ effect.key ] += effect.value;
		break;
	case EffectType::RemoveItem:
		ctx.player.inventory[ effect.key ] -= effect.value;
		if ( ctx.player.inventory[ effect.key ] <= 0 )
			ctx.player.inventory.erase(effect.key);
		break;
	}
}