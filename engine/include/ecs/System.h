#pragma once

#include <string>

using SystemID = std::string;

class System {
protected:
  System(SystemID id, int priority) noexcept;

public:
  static constexpr auto PRIORITY_LOW = 10000;
  static constexpr auto PRIORITY_MEDIUM = 1000;
  static constexpr auto PRIORITY_HIGH = 100;
  static constexpr auto NO_PRIORITY = -1;
  const SystemID id;
  const int priority;
  virtual ~System() = default;
  virtual void start() = 0;
  virtual void update(float dt) = 0;
};
