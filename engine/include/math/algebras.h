#ifndef MATH_ALGEBRAS_H
#define MATH_ALGEBRAS_H

namespace GGenie {

struct Vec2;
struct Vec3;
struct Quat;

struct Vec2 {
  float x, y;
};

struct Vec3 {
  float x, y, z;

  [[nodiscard]] auto length() const noexcept -> float;
  [[nodiscard]] auto normalize() const noexcept -> Vec3;
  [[nodiscard]] auto rotate(const Quat &quat) const noexcept -> Vec3;

  auto operator+(const Vec3 &other) const noexcept -> Vec3;
  auto operator-(const Vec3 &other) const noexcept -> Vec3;
  auto operator*(float value) const noexcept -> Vec3;
  auto operator/(float value) const noexcept -> Vec3;
  auto operator+=(const Vec3 &other) noexcept -> Vec3 &;
  auto operator-=(const Vec3 &other) noexcept -> Vec3 &;
  auto operator*=(float value) noexcept -> Vec3 &;
  auto operator/=(float value) noexcept -> Vec3 &;
  auto operator==(const Vec3 &other) const noexcept -> bool;
  auto operator!=(const Vec3 &other) const noexcept -> bool;

  static const Vec3 ZERO;
  static const Vec3 ONE;
};

struct Quat {
  float w, x, y, z;

  static const Quat IDENTITY;

  static auto fromAxisAngle(const Vec3 &axis, float radians) noexcept -> Quat;
  static auto fromEuler(float pitch, float yaw, float roll) noexcept -> Quat;

  [[nodiscard]] auto normalized() const -> Quat;
  [[nodiscard]] auto inverse() const -> Quat;
  // dot product or scalar product https://en.wikipedia.org/wiki/Dot_product
  [[nodiscard]] auto dot(const Quat &q) const noexcept -> float;

  auto operator+(const Quat &q) const noexcept -> Quat;
  auto operator-(const Quat &q) const noexcept -> Quat;
  auto operator*(const Quat &q) const noexcept -> Quat;
  auto operator/(const Quat &q) const noexcept -> Quat;
  auto operator*(float value) const noexcept -> Quat;
  auto operator/(float value) const noexcept -> Quat;
  auto operator==(const Quat &q) const noexcept -> bool;
  auto operator!=(const Quat &q) const noexcept -> bool;
};

} // namespace GGenie

#endif
