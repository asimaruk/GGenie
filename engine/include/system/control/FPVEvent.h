#ifndef SYSTEM_CONTROL_FPVEVENT_H
#define SYSTEM_CONTROL_FPVEVENT_H

#include "math/algebras.h"

namespace GGenie {

struct FPVMoveEvent {
  Vec3 direction;
};

struct FPVViewEvent {
  float xViewDiff, yViewDiff;
};

} // namespace GGenie

#endif
