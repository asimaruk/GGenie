#include "math/Mat4.h"
#include "math/Quat.h"
#include <algorithm>

Mat4::Mat4(std::array<float, 16> data) noexcept : data{data} {}

Mat4::Mat4(float fill) noexcept { std::fill(data.begin(), data.end(), fill); }

Mat4 Mat4::identity() noexcept {
  // clang-format off
  return Mat4({
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
  });
  // clang-format off
}

Mat4 Mat4::translation(const Vec3& v) noexcept {
  auto [ x, y, z ] = v;
  // clang-format off
  return Mat4({
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      x, y, z, 1
  });
  // clang-format on
}

Mat4 Mat4::rotation(const Quat &q) noexcept {
  auto [w, x, y, z] = q.normalized();

  float xx = x * x;
  float yy = y * y;
  float zz = z * z;
  float xy = x * y;
  float xz = x * z;
  float yz = y * z;
  float wx = w * x;
  float wy = w * y;
  float wz = w * z;

  return Mat4({1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz), 2.0f * (xz - wy), 0, 2.0f * (xy - wz),
               1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx), 0, 2.0f * (xz + wy), 2.0f * (yz - wx),
               1.0f - 2.0f * (xx + yy), 0, 0, 0, 0, 1});
}

Mat4 Mat4::scale(const Vec3& v) noexcept {
  auto [ x, y, z ] = v;
  // clang-format off
  return Mat4({
      x, 0, 0, 0,
      0, y, 0, 0,
      0, 0, z, 0,
      0, 0, 0, 1
  });
  // clang-format on
}

Mat4 Mat4::operator*(const Mat4 &other) const noexcept {
  Mat4 result = Mat4(0);
  for (int col = 0; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      float sum = 0.0f;
      for (int k = 0; k < 4; ++k) {
        sum += data[k * 4 + row] * other.data[col * 4 + k];
      }
      result.data[col * 4 + row] = sum;
    }
  }
  return result;
}

const float *Mat4::getData() const noexcept { return data.data(); }