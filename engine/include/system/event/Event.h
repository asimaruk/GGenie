#ifndef SYSTEM_EVENT_EVENT_H
#define SYSTEM_EVENT_EVENT_H

#include <typeindex>
#include <utility>

struct Event {
    virtual ~Event() = default;
    virtual std::type_index get_type() const = 0;
};

template<typename T>
struct TypedEvent : public Event {
    T data;
    
    explicit TypedEvent(T&& data) : data(std::forward<T>(data)) {}
    
    std::type_index get_type() const override {
        return typeid(T);
    }
};

#endif
