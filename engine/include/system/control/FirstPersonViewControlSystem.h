#ifndef SYSTEM_CONTROL_FIRSTPERSONVIEWCONTROLSYSTEM_H
#define SYSTEM_CONTROL_FIRSTPERSONVIEWCONTROLSYSTEM_H

#include "ecs/ComponentRegistry.hpp"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include "system/event/Event.h"
#include "system/event/EventSystem.hpp"
#include <memory>

class FirstPersonViewControlSystem : public System {
private:
  class Impl;
  std::unique_ptr<Impl> pimpl;

public:
  static constexpr auto ID = "FirstPersonViewControlSystemID";
  FirstPersonViewControlSystem(
      SystemID systemId,
      int priority,
      std::shared_ptr<ComponentRegistry> registry,
      std::shared_ptr<EventSystem> eventSystem
  );
  FirstPersonViewControlSystem(const FirstPersonViewControlSystem &) = delete;
  FirstPersonViewControlSystem(FirstPersonViewControlSystem &&) = delete;
  ~FirstPersonViewControlSystem() override;

  auto operator=(const FirstPersonViewControlSystem &) -> FirstPersonViewControlSystem & = delete;
  auto operator=(FirstPersonViewControlSystem &&) -> FirstPersonViewControlSystem & = delete;

  void start() override;
  void update(float dt) override;
  void setControlledEntity(Entity entity) noexcept;
};

#endif
