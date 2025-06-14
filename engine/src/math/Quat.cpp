#include "math/algebras.h"
#include <cmath>

const Quat Quat::IDENTITY = {.w = 1.0F, .x = 0.0F, .y = 0.0F, .z = 0.0F};

auto Quat::fromAxisAngle(const Vec3 &axis, float radians) noexcept -> Quat {
  auto halfAngle = radians / 2;
  auto sin = std::sin(halfAngle);
  return {.w = std::cos(halfAngle), .x = axis.x * sin, .y = axis.y * sin, .z = axis.z * sin};
}

auto Quat::fromEuler(float pitch, float yaw, float roll) noexcept -> Quat {
  // NOLINTBEGIN(readability-identifier-length)
  float const cr = cos(roll * 0.5F);
  float const sr = sin(roll * 0.5F);
  float const cp = cos(pitch * 0.5F);
  float const sp = sin(pitch * 0.5F);
  float const cy = cos(yaw * 0.5F);
  float const sy = sin(yaw * 0.5F);
  // NOLINTEND(readability-identifier-length)

  // Комбинирование вращений (Z-Y-X)
  return {
      .w = cr * cp * cy + sr * sp * sy,
      .x = sr * cp * cy - cr * sp * sy,
      .y = cr * sp * cy + sr * cp * sy,
      .z = cr * cp * sy - sr * sp * cy
  };
}

auto Quat::normalized() const -> Quat {
  float const len = sqrt(w * w + x * x + y * y + z * z);
  return Quat(w / len, x / len, y / len, z / len);
}

auto Quat::inverse() const -> Quat {
  float const norm = w * w + x * x + y * y + z * z;
  return Quat{.w = w / norm, .x = -x / norm, .y = -y / norm, .z = -z / norm};
}

auto Quat::dot(const Quat &q) const noexcept -> float {
  return w * q.w + x * q.x + y * q.y + z * q.z;
}

auto Quat::operator+(const Quat &q) const noexcept -> Quat {
  return {.w = w + q.w, .x = x + q.x, .y = y + q.y, .z = z + q.z};
}

auto Quat::operator-(const Quat &q) const noexcept -> Quat {
  return {.w = w - q.w, .x = x - q.x, .y = y - q.y, .z = z - q.z};
}

auto Quat::operator*(const Quat &q) const noexcept -> Quat {
  return Quat{
      .w = w * q.w - x * q.x - y * q.y - z * q.z,
      .x = w * q.x + x * q.w + y * q.z - z * q.y,
      .y = w * q.y - x * q.z + y * q.w + z * q.x,
      .z = w * q.z + x * q.y - y * q.x + z * q.w
  };
}

auto Quat::operator/(const Quat &q) const noexcept -> Quat {
  return *this * q.inverse();
}

auto Quat::operator*(float v) const noexcept -> Quat {
  return {.w = w * v, .x = x * v, .y = y * v, .z = z * v};
}

auto Quat::operator/(float v) const noexcept -> Quat {
  return *this * (1 / v);
}

auto Quat::operator==(const Quat &q) const noexcept -> bool {
  return w == q.w && x == q.x && y == q.y && z == q.z;
}
auto Quat::operator!=(const Quat &q) const noexcept -> bool {
  return !(*this == q);
}
