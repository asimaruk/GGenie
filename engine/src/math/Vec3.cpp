#include "math/algebras.h"
#include <cmath>

auto Vec3::length() const noexcept -> float {
  return std::sqrt((x * x) + (y * y) + (z * z));
}

auto Vec3::normalize() const noexcept -> Vec3 {
  auto len = length();
  if (len <= 1) {
    return *this;
  }
  return {.x = x / len, .y = y / len, .z = z / len};
}

auto Vec3::rotate(const Quat &quat) const noexcept -> Vec3 {
  auto rotated = quat * Quat{.w = 0, .x = x, .y = y, .z = z} * quat.inverse();
  return Vec3{.x = rotated.x, .y = rotated.y, .z = rotated.z};
}

auto Vec3::operator+(const Vec3 &other) const noexcept -> Vec3 {
  return Vec3{.x = x + other.x, .y = y + other.y, .z = z + other.z};
}

auto Vec3::operator-(const Vec3 &other) const noexcept -> Vec3 {
  return Vec3{.x = x - other.x, .y = y - other.y, .z = z - other.z};
}

auto Vec3::operator*(float value) const noexcept -> Vec3 {
  return Vec3{.x = x * value, .y = y * value, .z = z * value};
}

auto Vec3::operator/(float value) const noexcept -> Vec3 {
  return Vec3{.x = x / value, .y = y / value, .z = z / value};
}

auto Vec3::operator+=(const Vec3 &other) noexcept -> Vec3 & {
  *this = *this + other;
  return *this;
}

auto Vec3::operator-=(const Vec3 &other) noexcept -> Vec3 & {
  *this = *this - other;
  return *this;
}

auto Vec3::operator*=(float value) noexcept -> Vec3 & {
  *this = *this * value;
  return *this;
}

auto Vec3::operator/=(float value) noexcept -> Vec3 & {
  *this = *this / value;
  return *this;
}

auto Vec3::operator==(const Vec3 &other) const noexcept -> bool {
  return x == other.x && y == other.y && z == other.z;
}

auto Vec3::operator!=(const Vec3 &other) const noexcept -> bool {
  return !(*this == other);
}

const Vec3 Vec3::ZERO = Vec3{.x = 0, .y = 0, .z = 0};
const Vec3 Vec3::ONE = Vec3{.x = 1, .y = 1, .z = 1};
