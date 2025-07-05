#ifndef SYSTEM_CONTROL_FPVEVENT_H
#define SYSTEM_CONTROL_FPVEVENT_H

#include "math/algebras.h"

struct FPVMoveEvent {
  Vec3 direction;
};

struct FPVViewEvent {
  float xViewDiff, yViewDiff;
};

#endif
