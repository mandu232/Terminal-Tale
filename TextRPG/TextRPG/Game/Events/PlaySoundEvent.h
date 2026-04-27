#pragma
#include "Core/Event.h"
#include <string>

class PlaySoundEvent : public Event
{
public:
	std::string soundName;
};