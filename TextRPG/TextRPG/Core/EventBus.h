#pragma once

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <functional>
#include <memory>

#include "Event.h"

class EventBus
{
public:
	template<typename EventType>
	using Handler = std::function<void(const EventType&)>;

	template<typename EventType>
	void Subscribe(Handler<EventType> handler);

	template<typename EventType>
	void Emit(const EventType& event);

private:
	std::unordered_map<
		std::type_index,
		std::vector<std::function<void(const Event&)>>
	> subscribers;
};

template<typename EventType>
void EventBus::Subscribe(Handler<EventType> handler)
{
    auto wrapper =
        [handler](const Event& e)
        {
            handler(static_cast<const EventType&>(e));
        };

    subscribers[typeid(EventType)].push_back(wrapper);
}

template<typename EventType>
void EventBus::Emit(const EventType& event)
{
    auto it = subscribers.find(typeid(EventType));

    if (it == subscribers.end())
        return;

    for (auto& handler : it->second)
        handler(event);
}