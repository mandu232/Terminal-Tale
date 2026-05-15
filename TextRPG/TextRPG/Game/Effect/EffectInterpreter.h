#pragma once
#include <string>
#include "Effect.h"

class Context;

class EffectInterpreter
{
public:
	static void Apply(const Effect& effect , Context& ctx);
};