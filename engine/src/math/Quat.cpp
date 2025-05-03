#include "math/Quat.h"
#include <cmath>

Quat Quat::identity() noexcept { return Quat(1.0f, 0.0f, 0.0f, 0.0f); }

Quat Quat::fromAxisAngle(const Vec3 &axis, float radians) noexcept { return Quat(axis, radians); }

Quat Quat::fromEuler(float pitch, float yaw, float roll) noexcept { return Quat(pitch, yaw, roll); }

Quat::Quat(float w, float x, float y, float z) noexcept : data{w, x, y, z} {}

Quat::Quat(const Vec3 &axis, float radians) noexcept {
  auto halfAngle = radians / 2;
  auto sin = std::sin(halfAngle);
  data = {std::cos(halfAngle), axis.x() * sin, axis.y() * sin, axis.z() * sin};
}

Quat::Quat(float pitch, float yaw, float roll) noexcept {
  float cr = cos(roll * 0.5f);
  float sr = sin(roll * 0.5f);
  float cp = cos(pitch * 0.5f);
  float sp = sin(pitch * 0.5f);
  float cy = cos(yaw * 0.5f);
  float sy = sin(yaw * 0.5f);

  // Комбинирование вращений (Z-Y-X)
  data = {cr * cp * cy + sr * sp * sy, sr * cp * cy - cr * sp * sy, cr * sp * cy + sr * cp * sy,
          cr * cp * sy - sr * sp * cy};
}

float Quat::w() const noexcept { return data[0]; }

float Quat::x() const noexcept { return data[1]; }

float Quat::y() const noexcept { return data[2]; }

float Quat::z() const noexcept { return data[3]; }

Quat Quat::normalized() const {
  auto [w, x, y, z] = *this;
  float len = sqrt(w * w + x * x + y * y + z * z);
  return Quat(w / len, x / len, y / len, z / len);
}

float Quat::get(size_t index) const {
  if (index >= 4) {
    throw std::out_of_range("Quat index out of range");
  }
  return data[index];
}

// Structured binding support - compile-time checked access
template <size_t N> float get(const Quat &q) {
  static_assert(N < 4, "Quat has only 4 components");
  return q.get(N);
}

// Explicit template instantiations (required for linking)
template float get<0>(const Quat &); // w-component
template float get<1>(const Quat &); // x-component
template float get<2>(const Quat &); // y-component
template float get<3>(const Quat &); // z-component