#pragma once

#include "math/algebras.h"

struct Transform {
  Vec3 translation = Vec3::ZERO;
  Quat rotation = Quat::IDENTITY;
  Vec3 scale = Vec3::ONE;

  Transform lerp(const Transform& other, float t) const;

  Transform operator+(const Transform &other) const;
  Transform operator-(const Transform &other) const;
  Transform operator*(const float value) const;
  Transform operator/(const float value) const;
};
