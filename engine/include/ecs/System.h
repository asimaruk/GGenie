#pragma once

#include <string>

using SystemID = std::string;

class System {
protected:
  System(SystemID id, int priority) noexcept : id(id), priority(priority) {}

public:
  const SystemID id;
  const int priority;
  virtual ~System() = default;
  virtual void update(float dt) = 0;
};
