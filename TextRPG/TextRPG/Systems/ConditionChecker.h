#pragma once
#include <vector>
#include "Condition.h"

class Context;

class ConditionChecker
{
public:
	static bool Check(const std::vector<Condition>& conditions , const Context& ctx);

private:
	static bool CheckOne(const Condition& c , const Context& ctx);
};