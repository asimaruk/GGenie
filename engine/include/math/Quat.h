#pragma once

#include "Vec3.h"

struct Quat {
  float w, x, y, z;

  static const Quat IDENTITY;

  static Quat fromAxisAngle(const Vec3 &axis, float radians) noexcept;
  static Quat fromEuler(float pitch, float yaw, float roll) noexcept;

  Quat normalized() const;
  Quat inverse() const;
  // dot product or scalar product https://en.wikipedia.org/wiki/Dot_product
  float dot(const Quat &other) const noexcept;

  Quat operator+(const Quat &other) const noexcept;
  Quat operator-(const Quat &other) const noexcept;
  Quat operator*(const Quat &other) const noexcept;
  Quat operator/(const Quat &other) const noexcept;
  Quat operator*(float value) const noexcept;
  Quat operator/(float value) const noexcept;
};
