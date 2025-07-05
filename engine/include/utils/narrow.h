#ifndef UTILS_NARROW_H
#define UTILS_NARROW_H

#include <concepts>
#include <utility>

template <typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template <std::integral T, std::integral V> auto narrow(V value) -> T {
  if constexpr (std::is_same_v<T, V>) {
    return value;
  }
  return std::in_range<T>(value) ? static_cast<T>(value) : throw std::overflow_error("Narrowing error");
}

template <std::floating_point T, Arithmetic V> auto narrow(V value) -> T {
  return static_cast<T>(value);
}

#endif
