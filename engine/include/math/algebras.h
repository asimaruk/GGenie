#ifndef MATH_ALGEBRAS_H
#define MATH_ALGEBRAS_H

struct Vec2;
struct Vec3;
struct Quat;

struct Vec2 {
  float x, y;
};

struct Vec3 {
  float x, y, z;

  float length() const noexcept;
  Vec3 normalize() const noexcept;
  Vec3 rotate(const Quat &quat) const noexcept;

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

struct Quat {
  float w, x, y, z;

  static const Quat IDENTITY;

  static Quat fromAxisAngle(const Vec3 &axis, float radians) noexcept;
  static Quat fromEuler(float pitch, float yaw, float roll) noexcept;

  Quat normalized() const;
  Quat inverse() const;
  // dot product or scalar product https://en.wikipedia.org/wiki/Dot_product
  float dot(const Quat &q) const noexcept;

  Quat operator+(const Quat &q) const noexcept;
  Quat operator-(const Quat &q) const noexcept;
  Quat operator*(const Quat &q) const noexcept;
  Quat operator/(const Quat &q) const noexcept;
  Quat operator*(float value) const noexcept;
  Quat operator/(float value) const noexcept;
  bool operator==(const Quat &q) const noexcept;
  bool operator!=(const Quat &q) const noexcept;
};

#endif
