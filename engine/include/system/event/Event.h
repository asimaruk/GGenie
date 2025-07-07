#ifndef SYSTEM_EVENT_EVENT_H
#define SYSTEM_EVENT_EVENT_H

#include <typeindex>
#include <utility>

struct Event {
  Event() = default;
  Event(const Event &) = delete;
  Event(Event &&) = delete;
  virtual ~Event() = default;

  auto operator=(const Event &) -> Event & = delete;
  auto operator=(Event &&) -> Event & = delete;

  [[nodiscard]] virtual auto get_type() const -> std::type_index = 0;
};

template <typename T> struct TypedEvent : public Event {
  T data;

  // NOLINTBEGIN(cppcoreguidelines-rvalue-reference-param-not-moved)
  explicit TypedEvent(T &&data) : data(std::forward<T>(data)) {}
  // NOLINTEND(cppcoreguidelines-rvalue-reference-param-not-moved)
  TypedEvent(const TypedEvent &) = delete;
  TypedEvent(TypedEvent &&) = delete;
  ~TypedEvent() override = default;

  auto operator=(const TypedEvent &) -> TypedEvent & = delete;
  auto operator=(TypedEvent &&) -> TypedEvent & = delete;

  [[nodiscard]] auto get_type() const -> std::type_index override {
    return typeid(T);
  }
};

#endif
