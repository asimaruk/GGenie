#include "component/Transform.h"

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