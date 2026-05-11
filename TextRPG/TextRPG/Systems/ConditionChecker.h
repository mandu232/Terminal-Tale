#pragma once
#include <vector>
#include <string>

class Context;

class ConditionChecker
{
public:
	static bool Check(
		const std::vector<std::string>& conditions ,
		const Context& ctx
	);
};