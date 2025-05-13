#pragma once

struct Vec3 {
  float x, y, z;

  float length() const noexcept;
  Vec3 normalize() const noexcept;

  Vec3 operator+(const Vec3 &other) const noexcept;
  Vec3 operator-(const Vec3 &other) const noexcept;
  Vec3 operator*(float value) const noexcept;
  Vec3 operator/(float value) const noexcept;

  static const Vec3 ZERO;
  static const Vec3 ONE;
};
