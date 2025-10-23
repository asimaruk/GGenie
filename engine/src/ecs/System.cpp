#include "ecs/System.h"

using namespace GGenie;

System::System(SystemID id, int priority) noexcept : id(id), priority(priority) {}
