#pragma once
#include <string>

struct Achievement
{
    std::string id;
    std::string nameKey;   // lang 키
    std::string descKey;   // lang 키
    bool unlocked = false;
};
