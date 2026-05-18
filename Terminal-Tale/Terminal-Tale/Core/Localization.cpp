#include "Localization.h"
#include "Context.h"

Context* GContext = nullptr;

const std::string& L(const std::string& key)
{
	return GContext->localization.Get(key);
}