#pragma once

struct Vec3 {
  float x, y, z;

  Vec3 operator+(const Vec3 &other) const noexcept;
  static const Vec3 ZERO;
  static const Vec3 ONE;
};
