#ifndef SYSTEM_EVENT_EVENT_H
#define SYSTEM_EVENT_EVENT_H

#include <typeindex>
#include <utility>

struct Event {
    virtual ~Event() = default;
    [[nodiscard]] virtual auto get_type() const -> std::type_index = 0;
};

template<typename T>
struct TypedEvent : public Event {
    T data;
    
    explicit TypedEvent(T&& data) : data(std::forward<T>(data)) {}

    [[nodiscard]] auto get_type() const -> std::type_index override {
      return typeid(T);
    }
};

#endif
