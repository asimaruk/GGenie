#ifndef COMPONENT_TRANSFORM_H
#define COMPONENT_TRANSFORM_H

#include "math/algebras.h"

struct Transform {
  Vec3 translation = Vec3::ZERO;
  Quat rotation = Quat::IDENTITY;
  Vec3 scale = Vec3::ONE;

  [[nodiscard]] auto lerp(const Transform &other, float t) const -> Transform;

  auto operator+(const Transform &other) const -> Transform;
  auto operator-(const Transform &other) const -> Transform;
  auto operator*(float value) const -> Transform;
  auto operator/(float value) const -> Transform;
};

#endif
