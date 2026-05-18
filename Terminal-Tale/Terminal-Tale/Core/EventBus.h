#pragma once

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <functional>
#include <memory>

#include "Event.h"
#include "EventSubscription.h"

class EventBus
{
public:
    template<typename EventType>
    EventSubscription Subscribe(
        std::function<void(const EventType&)> handler);

    template<typename EventType>
    void Emit(const EventType& event);

    void Unsubscribe(size_t id);

private:
    struct Subscriber
    {
        size_t id;
        std::function<void(const Event&)> func;
    };

    std::unordered_map<
        std::type_index,
        std::vector<Subscriber>
    > subscribers;

    size_t nextId = 0;
};

template<typename EventType>
EventSubscription EventBus::Subscribe(
    std::function<void(const EventType&)> handler)
{
    size_t id = nextId++;

    auto wrapper =
        [handler](const Event& e)
        {
            handler(static_cast<const EventType&>(e));
        };

    subscribers[typeid(EventType)].push_back(
        { id, wrapper });

    return EventSubscription(this, id);
}

template<typename EventType>
void EventBus::Emit(const EventType& event)
{
    auto it = subscribers.find(typeid(EventType));

    if (it == subscribers.end())
        return;

    for (auto& sub : it->second)
        sub.func(event);
}