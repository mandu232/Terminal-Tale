#pragma once
#include <string>

class Context;

class EffectInterpreter
{
public:
	static void Apply(const std::string& ecffect , Context& ctx);
};