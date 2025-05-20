#pragma once
#include <algorithm>
#include <functional>
#include <mutex>
#include <queue>
#include <ranges>
#include <typeindex>

using ListenerId = size_t;

class IEventBase {
public:
    virtual ~IEventBase() = default;
    virtual void Flush() = 0;
};

template<typename T>
class Event final : public IEventBase
{
public:
    using Callback = std::function<void(const T&)>;
private:
    struct Listener
    {
        ListenerId Id;
        int Priority;
        Callback Callback;

        bool operator<(const Listener& other) const {
            return Priority < other.Priority;
        }
    };

    std::vector<Listener> Listeners;
    ListenerId NextId = 0;

    std::queue<T> QueuedEvents;
    mutable std::mutex ListenersLock;
    mutable std::mutex QueueLock;

public:
    ListenerId Register(Callback cb, int priority = 0) {
        std::lock_guard lock(ListenersLock);
        Listeners.push_back({ NextId, std::move(cb) });
        std::sort(Listeners.rbegin(), Listeners.rend());
        return NextId++;
    }

    void Unregister(ListenerId id) {
        std::lock_guard lock(ListenersLock);
        Listeners.erase(
            std::remove_if(Listeners.begin(), Listeners.end(),
                           [id](const Listener& l) { return l.id == id; }),
            Listeners.end());
    }

    void Dispatch(const T& event) const {
        std::vector<Callback> localCopy;
        {
            std::lock_guard lock(ListenersLock);
            for (const auto& l : Listeners)
                localCopy.push_back(l.Callback);
        }
        for (const auto& callback : localCopy)
            callback(event);
    }

    void QueueEvent(const T& event) {
        std::lock_guard lock(QueueLock);
        QueuedEvents.push(event);
    }

    void Flush() override {
        std::queue<T> tempQueue;
        {
            std::lock_guard lock(QueueLock);
            std::swap(tempQueue, QueuedEvents);
        }
        while (!tempQueue.empty()) {
            Dispatch(tempQueue.front());
            tempQueue.pop();
        }
    }
};

class EventBus
{
    std::unordered_map<std::type_index, std::unique_ptr<IEventBase>> Events;
    std::mutex EventsLock;

public:
    static EventBus& Instance() {
        static EventBus instance;
        return instance;
    }

    template<typename T>
    Event<T>& Get()
    {
        std::lock_guard lock(EventsLock);
        const auto key = std::type_index(typeid(T));
        if (!Events.contains(key))
        {
            Events[key] = std::make_unique<Event<T>>();
        }
        return *static_cast<Event<T> *>(Events[key].get());
    }

    void FlushAll() {
        std::lock_guard lock(EventsLock);
        for (const auto &event : Events | std::views::values)
            event->Flush();
    }

    void ClearAll()
    {
        std::lock_guard lock(EventsLock);
        Events.clear();
    }

    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

private:
    EventBus() = default;
    ~EventBus() = default;
};
