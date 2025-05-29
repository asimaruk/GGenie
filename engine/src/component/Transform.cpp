#include "component/Transform.h"
#include "math/lerp.hpp"

auto Transform::lerp(const Transform &other, float t) const -> Transform {
  return {
      .translation = math::lerp(translation, other.translation, t),
      .rotation = math::slerp(rotation, other.rotation, t),
      .scale = math::lerp(scale, other.scale, t)
  };
}

auto Transform::operator+(const Transform &other) const -> Transform {
  return Transform{
      .translation = translation + other.translation,
      .rotation = rotation + other.rotation,
      .scale = scale + other.scale,
  };
}

auto Transform::operator-(const Transform &other) const -> Transform {
  return Transform{
      .translation = translation - other.translation,
      .rotation = rotation - other.rotation,
      .scale = scale - other.scale,
  };
}

auto Transform::operator*(float value) const -> Transform {
  return Transform{
      .translation = translation * value,
      .rotation = rotation * value,
      .scale = scale * value,
  };
}

auto Transform::operator/(float value) const -> Transform {
  return Transform{
      .translation = translation / value,
      .rotation = rotation / value,
      .scale = scale / value,
  };
}
