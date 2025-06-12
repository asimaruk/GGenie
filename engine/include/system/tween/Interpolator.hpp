#pragma once

#include "easing.hpp"
#include "math/algebras.h"
#include "math/lerp.hpp"
#include <__ostream/print.h>
#include <functional>

class Interpolator {
private:
  const std::function<float(float)> ease;

public:
  static const Interpolator LinearInterpolator;

  Interpolator(std::function<float(float)> ease) : ease(ease) {};

  template <math::Lerpable T>
  T operator()(const T &a, const T &b, float t) const {
    if constexpr (math::HasLerpMethod<T>) {
      return a.lerp(b, ease(t));
    } else {
      return a + (b - a) * ease(t);
    }
  }

  template <> Quat operator()(const Quat &a, const Quat &b, float t) const {
    return math::slerp(a, b, ease(t));
  }
};

inline const Interpolator Interpolator::LinearInterpolator =
    Interpolator(easing::LINEAR);
