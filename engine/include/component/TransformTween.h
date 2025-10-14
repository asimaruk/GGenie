#ifndef COMPONENT_TRANSFORM_TWEEN_H
#define COMPONENT_TRANSFORM_TWEEN_H

#include "Transform.h"

struct TransformTween {
  static constexpr int INFINITY_REPEAT = -1;
  Transform from, to;
  float duration;
  float time;
  int repeat;
};

#endif
