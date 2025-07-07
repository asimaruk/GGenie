#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H

#include <string_view>

using SystemID = std::string_view;

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

  System(const System &) = delete;
  System(System &&) = delete;

  auto operator=(const System &) -> System & = delete;
  auto operator=(System &&) -> System & = delete;

  virtual ~System() = default;
  virtual void start() = 0;
  virtual void update(float dt) = 0;
};

#endif
