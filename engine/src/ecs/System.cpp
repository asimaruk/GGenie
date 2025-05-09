#include "ecs/System.h"

System::System(SystemID id, int priority) noexcept : id(id), priority(priority) {}