#include "math/Vec3.h"

Vec3 Vec3::operator+(const Vec3 &other) const noexcept {
  return Vec3{.x = x + other.x, .y = y + other.y, .z = z + other.z};
}

const Vec3 Vec3::ZERO = Vec3{0, 0, 0};
const Vec3 Vec3::ONE = Vec3{1, 1, 1};