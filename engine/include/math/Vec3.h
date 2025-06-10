#pragma once

struct Vec3 {
  float x, y, z;

  float length() const noexcept;
  Vec3 normalize() const noexcept;

  Vec3 operator+(const Vec3 &other) const noexcept;
  Vec3 operator-(const Vec3 &other) const noexcept;
  Vec3 operator*(float value) const noexcept;
  Vec3 operator/(float value) const noexcept;
  Vec3 &operator+=(const Vec3 &other) noexcept;
  Vec3 &operator-=(const Vec3 &other) noexcept;
  Vec3 &operator*=(float value) noexcept;
  Vec3 &operator/=(float value) noexcept;
  bool operator==(const Vec3 &other) const noexcept;
  bool operator!=(const Vec3 &other) const noexcept;

  static const Vec3 ZERO;
  static const Vec3 ONE;
};
