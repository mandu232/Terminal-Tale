#pragma once
#include <string>
#include "Effect.h"

class Context;

class EffectInterpreter
{
public:
	static void Apply(const std::string& effect , Context& ctx);
	static void Apply(const Effect& effect , Context& ctx);
};