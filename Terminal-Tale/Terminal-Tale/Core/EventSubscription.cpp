#include "EventSubscription.h"
#include "EventBus.h"

EventSubscription::EventSubscription(
	EventBus* bus,
	size_t id)
	: bus(bus), subscriptionId(id)
{
}

EventSubscription::~EventSubscription()
{
    if (bus)
        bus->Unsubscribe(subscriptionId);
}

EventSubscription::EventSubscription(EventSubscription&& other) noexcept
{
    bus = other.bus;
    subscriptionId = other.subscriptionId;

    other.bus = nullptr;
}

EventSubscription&
EventSubscription::operator=(EventSubscription&& other) noexcept
{
    if (this != &other)
    {
        bus = other.bus;
        subscriptionId = other.subscriptionId;
        other.bus = nullptr;
    }
    return *this;
}