#include "component/Transform.h"
#include "math/lerp.hpp"

Transform Transform::lerp(const Transform& other, float t) const {
  return {
    .translation = math::lerp(translation, other.translation, t),
    .rotation = math::slerp(rotation, other.rotation, t),
    .scale = math::lerp(scale, other.scale, t)
  };
}

Transform Transform::operator+(const Transform &other) const {
  return Transform{
      .translation = translation + other.translation,
      .rotation = rotation + other.rotation,
      .scale = scale + other.scale,
  };
}

Transform Transform::operator-(const Transform &other) const {
  return Transform{
      .translation = translation - other.translation,
      .rotation = rotation - other.rotation,
      .scale = scale - other.scale,
  };
}

Transform Transform::operator*(float value) const {
  return Transform{
      .translation = translation * value,
      .rotation = rotation * value,
      .scale = scale * value,
  };
}

Transform Transform::operator/(float value) const {
  return Transform{
      .translation = translation / value,
      .rotation = rotation / value,
      .scale = scale / value,
  };
}