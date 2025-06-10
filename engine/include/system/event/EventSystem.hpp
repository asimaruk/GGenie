#pragma once

#include "ecs/System.h"
#include "Event.h"
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

using EventListener = std::function<void(const Event &)>;

class EventSystem {
public:
  template <typename T> void dispatch(T &&eventData) {
    TypedEvent<T> event(std::forward<T>(eventData));
    auto type = event.get_type();
    if (auto it = listeners.find(type); it != listeners.end()) {
      for (auto &listener : it->second) {
        listener(event);
      }
    }
  }

  template <typename T> void on(std::function<void(const T &)> handler) {
    const auto listener = [handler = std::move(handler)](const Event &event)
    {
      handler(static_cast<const TypedEvent<T> &>(event).data);
    };
    listeners[typeid(T)].push_back(listener);
  }

  template <typename T> void off(EventListener listener) {}

private:
  std::unordered_map<std::type_index, std::vector<EventListener>> listeners;
};
