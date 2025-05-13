#include "math/Vec3.h"
#include <cmath>

float Vec3::length() const noexcept { return std::sqrt(x * x + y * y + z * z); }

Vec3 Vec3::normalize() const noexcept {
  auto len = length();
  if (len <= 1) {
    return *this;
  }
  return {.x = x / len, .y = y / len, .z = z / len};
}

Vec3 Vec3::operator+(const Vec3 &other) const noexcept {
  return Vec3{.x = x + other.x, .y = y + other.y, .z = z + other.z};
}

Vec3 Vec3::operator-(const Vec3 &other) const noexcept {
  return Vec3{.x = x - other.x, .y = y - other.y, .z = z - other.z};
}

Vec3 Vec3::operator*(float value) const noexcept {
  return Vec3{.x = x * value, .y = y * value, .z = z * value};
}

Vec3 Vec3::operator/(float value) const noexcept {
  return Vec3{.x = x / value, .y = y / value, .z = z / value};
}

const Vec3 Vec3::ZERO = Vec3{0, 0, 0};
const Vec3 Vec3::ONE = Vec3{1, 1, 1};