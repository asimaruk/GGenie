#ifndef COMPONENT_TRANSFORM_TWEEN_H
#define COMPONENT_TRANSFORM_TWEEN_H

#include "Transform.h"

struct TransformTween {
    Transform from, to;
    float duration;
    float time;
};

#endif