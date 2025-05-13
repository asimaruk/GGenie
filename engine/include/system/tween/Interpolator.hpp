#pragma once

#include "easing.hpp"
#include "math/Quat.h"
#include <concepts>
#include <functional>

template <typename T>
concept Lerpable = requires(const T &a, const T &b, float t) {
    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    { a * t } -> std::same_as<T>;
};

class Interpolator {
private:
  const std::function<float(float)> ease;

public:
  static const Interpolator LinearInterpolator;

  Interpolator(std::function<float(float)> ease) : ease(ease) {};

  template <Lerpable T> T operator()(const T &start, const T &end, float t) const {
    return start + (end - start) * ease(t);
  }
};

inline const Interpolator Interpolator::LinearInterpolator = Interpolator(Easing::LINEAR);
