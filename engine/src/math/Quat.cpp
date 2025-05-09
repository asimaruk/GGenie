#include "math/Quat.h"
#include <cmath>

const Quat Quat::IDENTITY = {.w=1.0f, .x=0.0f, .y=0.0f, .z=0.0f};

Quat Quat::fromAxisAngle(const Vec3 &axis, float radians) noexcept {
  auto halfAngle = radians / 2;
  auto sin = std::sin(halfAngle);
  return {.w = std::cos(halfAngle),
          .x = axis.x * sin,
          .y = axis.y * sin,
          .z = axis.z * sin};
}

Quat Quat::fromEuler(float pitch, float yaw, float roll) noexcept {
  float cr = cos(roll * 0.5f);
  float sr = sin(roll * 0.5f);
  float cp = cos(pitch * 0.5f);
  float sp = sin(pitch * 0.5f);
  float cy = cos(yaw * 0.5f);
  float sy = sin(yaw * 0.5f);

  // Комбинирование вращений (Z-Y-X)
  return {.w = cr * cp * cy + sr * sp * sy,
          .x = sr * cp * cy - cr * sp * sy,
          .y = cr * sp * cy + sr * cp * sy,
          .z = cr * cp * sy - sr * sp * cy};
}

Quat Quat::normalized() const {
  float len = sqrt(w * w + x * x + y * y + z * z);
  return Quat(w / len, x / len, y / len, z / len);
}
