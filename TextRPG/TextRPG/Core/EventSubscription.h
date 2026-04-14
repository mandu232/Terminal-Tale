#pragma once

#include <functional>

class EventBus;

class EventSubscription
{
public:
	EventSubscription() = default;

	EventSubscription(
		EventBus* bus,
		size_t id);

	~EventSubscription();

	EventSubscription(const EventSubscription&) = delete;
	EventSubscription& operator=(const EventSubscription&) = delete;

	EventSubscription(EventSubscription&& other) noexcept;
	EventSubscription& operator=(EventSubscription&& other) noexcept;

private:
	EventBus* bus = nullptr;
	size_t subscriptionId = 0;
};