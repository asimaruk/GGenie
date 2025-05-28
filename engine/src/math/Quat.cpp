#include "math/Quat.h"
#include <cmath>

const Quat Quat::IDENTITY = {.w = 1.0f, .x = 0.0f, .y = 0.0f, .z = 0.0f};

Quat Quat::fromAxisAngle(const Vec3 &axis, float radians) noexcept {
  auto halfAngle = radians / 2;
  auto sin = std::sin(halfAngle);
  return {.w = std::cos(halfAngle), .x = axis.x * sin, .y = axis.y * sin, .z = axis.z * sin};
}

Quat Quat::fromEuler(float pitch, float yaw, float roll) noexcept {
  float cr = cos(roll * 0.5f);
  float sr = sin(roll * 0.5f);
  float cp = cos(pitch * 0.5f);
  float sp = sin(pitch * 0.5f);
  float cy = cos(yaw * 0.5f);
  float sy = sin(yaw * 0.5f);

  // Комбинирование вращений (Z-Y-X)
  return {
      .w = cr * cp * cy + sr * sp * sy,
      .x = sr * cp * cy - cr * sp * sy,
      .y = cr * sp * cy + sr * cp * sy,
      .z = cr * cp * sy - sr * sp * cy
  };
}

Quat Quat::normalized() const {
  float len = sqrt(w * w + x * x + y * y + z * z);
  return Quat(w / len, x / len, y / len, z / len);
}

Quat Quat::inverse() const {
  float norm = w * w + x * x + y * y + z * z;
  return Quat{.w = w / norm, .x = -x / norm, .y = -y / norm, .z = -z / norm};
}

float Quat::dot(const Quat &q) const noexcept {
  return w * q.w + x * q.x + y * q.y + z * q.z;
}

Quat Quat::operator+(const Quat &q) const noexcept {
  return {.w = w + q.w, .x = x + q.x, .y = y + q.y, .z = z + q.z};
}

Quat Quat::operator-(const Quat &q) const noexcept {
  return {.w = w - q.w, .x = x - q.x, .y = y - q.y, .z = z - q.z};
}

Quat Quat::operator*(const Quat &q) const noexcept {
  return Quat{
      .w = w * q.w - x * q.x - y * q.y - z * q.z,
      .x = w * q.x + x * q.w + y * q.z - z * q.y,
      .y = w * q.y - x * q.z + y * q.w + z * q.x,
      .z = w * q.z + x * q.y - y * q.x + z * q.w
  };
}

Quat Quat::operator/(const Quat &q) const noexcept {
  return *this * q.inverse();
}

Quat Quat::operator*(float v) const noexcept {
  return {.w = w * v, .x = x * v, .y = y * v, .z = z * v};
}

Quat Quat::operator/(float v) const noexcept {
  return *this * (1 / v);
}
