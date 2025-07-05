
#ifndef MATH_LERP_HPP
#define MATH_LERP_HPP

#include "math/algebras.h"
#include <algorithm>
#include <cmath>
#include <concepts>

namespace math {

template <typename T>
concept HasLerpMethod = requires(const T &a, const T &b, float t) {
  { a.lerp(b, t) } -> std::same_as<T>;
};

template <typename T>
concept HasArithmeticOps = requires(const T &a, const T &b, float t) {
  { a + b } -> std::same_as<T>;
  { a - b } -> std::same_as<T>;
  { a * t } -> std::same_as<T>;
};

template <typename T>
concept Lerpable = HasLerpMethod<T> || HasArithmeticOps<T>;

template <Lerpable T> auto lerp(const T &a, const T &b, float t) -> T {
  return a + (b - a) * t;
}

// https://en.wikipedia.org/wiki/Slerp
inline auto slerp(const Quat &a, const Quat &b, float t) -> Quat {
  float cosTheta = a.dot(b);
  float theta = std::acos(std::clamp(cosTheta, -1.0f, 1.0f));
  float sinTheta = std::sin(theta);
  float wa = std::sin((1 - t) * theta) / sinTheta;
  float wb = std::sin(t * theta) / sinTheta;
  return Quat{wa * a.w + wb * b.w, wa * a.x + wb * b.x, wa * a.y + wb * b.y,
              wa * a.z + wb * b.z}
      .normalized();
}

} // namespace math

#endif
